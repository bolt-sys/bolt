#include "Utilities.h"

STATUS
SYSAPI
AllocateEmptySlab (
    IN OUT SLAB_CACHE* Cache
    )
{
    STATUS Status;
    UINTN  SlabPages;
    UINTN  SlabDataPages;
    SLAB*  Slab;

    Status = STATUS_SUCCESS;

    //
    // Allocate a new slab, we can just use the remaining page space of the
    // slab for the freemap, since the slab itself is really small.
    //
    SlabPages     = ALIGN_UP (sizeof (SLAB), PAGE_SIZE_4K) / PAGE_SIZE_4K;
    SlabDataPages = ALIGN_UP ((Cache->ObjectSize * Cache->ObjectCount), PAGE_SIZE_4K) / PAGE_SIZE_4K;

    Status = AllocatePages ((VOID**)&Slab, SlabPages + SlabDataPages, PAGE_SIZE_4K);
    if (FAILED (Status)) {
        goto Exit;
    }

    Slab->FreeMap    = (VOID*)(Slab + 1);
    Slab->FreeMapEnd = ((VOID*)Slab) + (SlabPages * PAGE_SIZE_4K);
    Slab->Data       = (VOID*)Slab->FreeMapEnd;
    Slab->DataEnd    = (VOID*)Slab->Data + (SlabDataPages * PAGE_SIZE_4K);

    // Mark the whole free map as full
    FillMemory (Slab->FreeMap, Slab->FreeMapEnd - Slab->FreeMap, 0xFF);

    // Mark all objects as free
    for (UINTN i = 0; i < Cache->ObjectCount; i++) {
        BIT_CLEAR (Slab->FreeMap[i / 8], i % 8);
    }

    Slab->FreeCount = Cache->ObjectCount;

    Slab->Cache = Cache;
    MoveSlabToList (Slab, &Cache->Empty);

Exit:
    return Status;
}

/**
 * @brief Moves a slab from one list to another.
 *
 * @param[in,out] Slab The slab to move.
 * @param[in,out] List The list to move the slab to.
 *
 * @return STATUS_SUCCESS on success.
 */
STATUS
SYSAPI
MoveSlabToList (
    IN OUT SLAB*  Slab,
    IN OUT SLAB** List
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    //
    // we first need to remove the slab from its current list
    //
    if (Slab->Prev != NULL) {
        Slab->Prev->Next = Slab->Next;
        Slab->Prev       = NULL;
    }

    if (Slab->Next != NULL) {
        Slab->Next->Prev = Slab->Prev;
        Slab->Next       = NULL;
    }

    //
    // now we can add it to the new list
    //
    if (*List != NULL) {
        (*List)->Prev = Slab;
    }

    Slab->Next = *List;
    Slab->Prev = NULL;
    *List      = Slab;

    return Status;
}

STATUS
SYSAPI
FindSlabForAllocation (
    IN SLAB_CACHE* Cache,
    OUT SLAB**     Slab
    )
{
    STATUS Status;
    SLAB*  CurrentSlab;

    Status = STATUS_SUCCESS;

    //
    // Check partial slabs first
    //
    CurrentSlab = Cache->Partial;
    if (CurrentSlab != NULL) {
        *Slab = CurrentSlab;
        goto Exit;
    }

    //
    // Check empty slabs next
    //
    CurrentSlab = Cache->Empty;
    if (CurrentSlab != NULL) {
        *Slab = CurrentSlab;
        goto Exit;
    }

    //
    // uh oh, no slabs found
    //
    Status = STATUS_NOT_FOUND;

Exit:
    return Status;
}

/**
 * @brief Find an empty object in a slab
 *
 * @param[in]  Slab   The slab to search
 * @param[out] Bit    The bit of the object within the freemap
 *
 * @return STATUS_SUCCESS              The search was successful
 * @return STATUS_INVALID_PARAMETER    Slab or Object is NULL
 * @return STATUS_NOT_FOUND            No empty object was found
 **/
STATUS
SYSAPI
FindEmptyObjectInSlab (
    IN  SLAB*  Slab,
    OUT UINTN* Bit
    )
{
    STATUS Status;
    UINTN  i;

    Status = STATUS_SUCCESS;

    if ((Slab == NULL) || (Bit == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    //
    // Search the freemap for an empty object
    //
    for (i = 0; i < Slab->Cache->ObjectCount; i++) {
        if (BIT_IS_CLEAR (Slab->FreeMap[i / 8], i % 8)) {
            *Bit = i;
            goto Exit;
        }
    }

    //
    // This should never happen
    //
    Status = STATUS_NOT_FOUND;

Exit:
    return Status;
}

/**
 * @brief Allocate an object from a slab
 *
 * @param[in]  Slab   The slab to allocate from
 * @param[in]  Flags  The flags for the allocation
 * @param[out] Object The object to allocate
 *
 * @return STATUS_SUCCESS              The allocation was successful
 * @return STATUS_INVALID_PARAMETER    Object or Slab is NULL
 * @return STATUS_OUT_OF_MEMORY        The slab is full
 *
 **/
STATUS
SYSAPI
AllocateObjectFromSlab (
    IN  SLAB*  Slab,
    IN  UINTN  Flags,
    OUT VOID** Object
    )
{
    STATUS Status;
    UINTN  Bit;

    Status = STATUS_SUCCESS;

    if ((Slab == NULL) || (Object == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Slab->FreeCount == 0) {
        // we can just skip the whole search if the slab is full
        Status = STATUS_OUT_OF_MEMORY;
        goto Exit;
    }

    //
    // Find an empty object
    //
    Status = FindEmptyObjectInSlab (Slab, &Bit);
    if (FAILED (Status)) {
        goto Exit;
    }

    //
    // Mark the object as used
    //
    BIT_SET (Slab->FreeMap[Bit / 8], Bit % 8);
    Slab->FreeCount--;

    //
    // Calculate the address of the object
    //
    *Object = (VOID*)((UINTN)Slab->Data + (Bit * Slab->Cache->ObjectSize));

    //
    // Setup the object
    //
    Slab->Cache->Ctor (*Object, Slab->Cache, Flags);

    //
    // Move into the appropriate list
    //
    // BUGFIX: if we're the first slab in the empty, partial or full list is ours,
    //         we need to temporarily remove it from the list.
    if (Slab->Cache->Empty == Slab) {
        Slab->Cache->Empty = Slab->Next;
    } else if (Slab->Cache->Partial == Slab) {
        Slab->Cache->Partial = Slab->Next;
    } else if (Slab->Cache->Full == Slab) {
        Slab->Cache->Full = Slab->Next;
    }

    if (Slab->FreeCount == 0) {
        MoveSlabToList (Slab, &Slab->Cache->Full);
    } else {
        MoveSlabToList (Slab, &Slab->Cache->Partial);
    }

Exit:
    return Status;
}

STATUS
SYSAPI
FindSlabForObject (
    IN SLAB_CACHE* Cache,
    IN VOID*       Object,
    OUT SLAB**     Slab,
    OUT UINTN*     Bit
    )
{
    STATUS Status;
    SLAB*  CurrentSlab;
    
    Status = STATUS_SUCCESS;
    
    if ((Cache == NULL) || (Object == NULL) || (Slab == NULL) || (Bit == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }
    
    //
    // Search the partial slabs first
    //
    CurrentSlab = Cache->Partial;
    while (CurrentSlab != NULL) {
        // Check if the object is within the slab data range
        if ((Object >= CurrentSlab->Data) && (Object < CurrentSlab->DataEnd)) {
            *Slab = CurrentSlab; // Found it!
            
            // Calculate the correct bit offset
            *Bit  = ((UINTN)Object - (UINTN)CurrentSlab->Data) / Cache->ObjectSize;
            goto Exit;
        }
        
        CurrentSlab = CurrentSlab->Next; // Keep searching
    }
    
    //
    // Search the full slabs next
    //
    CurrentSlab = Cache->Full;
    
    while (CurrentSlab != NULL) {
        // Check if the object is within the slab data range
        if ((Object >= CurrentSlab->Data) && (Object < CurrentSlab->DataEnd)) {
            *Slab = CurrentSlab; // Found it!
            
            // Calculate the correct bit offset
            *Bit  = ((UINTN)Object - (UINTN)CurrentSlab->Data) / Cache->ObjectSize;
            goto Exit;
        }
        
        CurrentSlab = CurrentSlab->Next; // Keep searching
    }
    
    //
    // uh oh, no slabs found
    //
    Status = STATUS_NOT_FOUND;
    
Exit:
    return Status;
}

STATUS
SYSAPI
FreeObjectInSlab (
    IN OUT SLAB* Slab,
    IN     UINTN Flags,
    IN     UINTN Bit
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if (Slab == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    //
    // Mark the object as free
    //
    BIT_CLEAR (Slab->FreeMap[Bit / 8], Bit % 8);
    Slab->FreeCount++;

    //
    // Call the destructor
    //
    Slab->Cache->Dtor ((VOID*)((UINTN)Slab->Data + (Bit * Slab->Cache->ObjectSize)), Slab->Cache, Flags);

    //
    // Move into the appropriate list
    //
    // BUGFIX: if we're the first slab in the empty, partial or full list is ours,
    //         we need to temporarily remove it from the list.
    if (Slab->Cache->Empty == Slab) {
        Slab->Cache->Empty = Slab->Next;
    } else if (Slab->Cache->Partial == Slab) {
        Slab->Cache->Partial = Slab->Next;
    } else if (Slab->Cache->Full == Slab) {
        Slab->Cache->Full = Slab->Next;
    }

    if (Slab->FreeCount == Slab->Cache->ObjectCount) {
        MoveSlabToList (Slab, &Slab->Cache->Empty);
    } else {
        MoveSlabToList (Slab, &Slab->Cache->Partial);
    }
    
Exit:
    return Status;
}
