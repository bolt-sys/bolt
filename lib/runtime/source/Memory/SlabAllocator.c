#include <RuntimeLib.h>

#include "SlabAllocatorUtilities.h"

// TODO: Require a buddy allocator to be used.
//       but its not yet implemented.

// ----------------------------------------------------------------- Slab Cache

#define MINIMUM_OBJECT_COUNT 16 // At least 16 objects per slab
// to fight against inefficiency when using
// an object larger than a page.
//
// Although this will consume more memory,
// if not all objects are used.

/**
 * @brief Create a slab cache
 *
 * @param[out]         Cache         The cache to initialize
 * @param[in]          BaseAllocator The base allocator to use
 * @param[in]          ObjectSize    The size of the objects to be allocated
 * @param[in,optional] Flags         The flags for the cache
 * @param[in,optional] Ctor          The constructor for the objects
 * @param[in,optional] Dtor          The destructor for the objects
 *
 * @return STATUS_SUCCESS              The initialization was successful
 * @return STATUS_INVALID_PARAMETER    Cache is NULL or ObjectSize is 0
 * @return STATUS_NOT_INITIALIZED      The Page Allocator is not initialized
 **/
STATUS
SYSAPI
RtlCreateSlabCache (
    OUT         SLAB_CACHE** Cache,
    IN PAGE_ALLOCATOR*       BaseAllocator,
    IN          UINTN        ObjectSize,
    IN OPTIONAL UINTN        Flags,
    IN OPTIONAL SLAB_CTOR    Ctor,
    IN OPTIONAL SLAB_DTOR    Dtor
    )
{
    STATUS      Status;
    SLAB_CACHE* CachePtr;
    UINTN       ObjectCount;

    Status = STATUS_SUCCESS;

    if ((Cache == NULL) || (ObjectSize == 0)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Ctor == NULL) {
        Ctor = RtlDefaultSlabCtor;
    }

    if (Dtor == NULL) {
        Dtor = RtlDefaultSlabDtor;
    }

    Status = PA_ALLOCATE (BaseAllocator, (VOID**)Cache, 1, PAGE_SIZE_4K);
    if (FAILED (Status)) {
        goto Exit;
    }

    CachePtr = *Cache;

    RtlZeroMemory (CachePtr, sizeof (SLAB_CACHE));

    ObjectCount = ALIGN_UP (PAGE_SIZE_4K, ObjectSize) / ObjectSize;

    CachePtr->ObjectSize    = ObjectSize;
    CachePtr->ObjectCount   = MAX (ObjectCount, MINIMUM_OBJECT_COUNT);
    CachePtr->Flags         = Flags;
    CachePtr->Empty         = NULL;
    CachePtr->Partial       = NULL;
    CachePtr->Full          = NULL;
    CachePtr->Ctor          = Ctor;
    CachePtr->Dtor          = Dtor;
    CachePtr->BaseAllocator = BaseAllocator;

Exit:
    return Status;
}

/**
 * @brief Destroy a slab cache
 *
 * @param[in,out] Cache The cache to destroy
 *
 * @return STATUS_SUCCESS              The destruction was successful
 * @return STATUS_INVALID_PARAMETER    Cache is NULL
 * @return STATUS_NOT_INITIALIZED      The Page Allocator is not initialized
 **/
STATUS
SYSAPI
RtlDestroySlabCache (
    IN OUT SLAB_CACHE** Cache
    )
{
    STATUS      Status;
    SLAB_CACHE* CachePtr;

    Status = STATUS_SUCCESS;

    if (Cache == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    CachePtr = *Cache;

    if (CachePtr == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (CachePtr->Empty != NULL || CachePtr->Partial != NULL || CachePtr->Full != NULL) {
        SLAB* Slab;
        SLAB* Next;

        //
        // Free full slabs
        //
        Slab = CachePtr->Full;
        while (Slab != NULL) {
            Next = Slab->Next; // Save the next slab
            DestroySlab(Slab);
            Slab = Next;
        }

        //
        // Free partial slabs
        //
        Slab = CachePtr->Partial;
        while (Slab != NULL) {
            Next = Slab->Next;
            DestroySlab(Slab);
            Slab = Next;
        }

        //
        // Free empty slabs
        //
        Slab = CachePtr->Empty;
        while (Slab != NULL) {
            Next = Slab->Next;
            DestroySlab(Slab);
            Slab = Next;
        }
    }

    Status = PA_FREE (CachePtr->BaseAllocator, (VOID**)Cache, 1, PAGE_SIZE_4K);
    if (FAILED (Status)) {
        goto Exit;
    }

    *Cache = NULL;

Exit:
    return Status;
}

// ------------------------------------------------------------ Slab SlabAllocation

/**
 * @brief Allocate an object from a slab cache
 *
 * @param[out] Object The object to allocate
 * @param[in]  Cache The cache to allocate from
 * @param[in]  Flags The flags for the allocation
 *
 * @return STATUS_SUCCESS              The allocation was successful
 * @return STATUS_INVALID_PARAMETER    Object or Cache is NULL
 *
 **/
STATUS
SYSAPI
RtlSlabAllocate (
    IN  SLAB_CACHE* Cache,
    IN  UINTN       Flags,
    OUT VOID**      Object
    )
{
    STATUS Status;
    SLAB*  Slab;

    Status = STATUS_SUCCESS;

    if ((Cache == NULL) || (Object == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Slab = NULL;

    //
    // First we want to check if there are any partially filled slabs
    // If there are, we want to allocate from them.
    //
    Status = FindSlabForAllocation (Cache, &Slab);
    if (FAILED (Status)) {
        if (Status != STATUS_NOT_FOUND) {
            goto Exit; // Unexpected error
        }

        //
        // If we didn't find one, we have to allocate a new slab.
        //
        Status = AllocateEmptySlab (Cache);
        if (FAILED (Status)) {
            goto Exit;
        }

        //
        // Now that we have a new slab, we want to try to find that slab again.
        //
        Status = FindSlabForAllocation (Cache, &Slab);
        if (FAILED (Status)) {
            // This should never happen. but if it does, this is a safeguard.
            goto Exit;
        }
    }

    //
    // Now that we have a slab, we want to allocate an object from it.
    //
    Status = AllocateObjectFromSlab (Slab, Flags, Object);
    if (FAILED (Status)) {
        goto Exit;
    }

Exit:
    return Status;
}

/**
 * @brief Free an object from a slab cache
 *
 * @param[in]     Cache The cache to free from
 * @param[in]     Flags The flags for the free
 * @param[in,out] Object The object to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Object or Cache is NULL
 * @return STATUS_NOT_FOUND            The object was not found in the cache
 **/
STATUS
SYSAPI
RtlSlabFree (
    IN     SLAB_CACHE* Cache,
    IN     UINTN       Flags,
    IN OUT VOID**      Object
    )
{
    STATUS Status;
    SLAB*  Slab;
    UINTN  Bit;

    Status = STATUS_SUCCESS;

    if ((Object == NULL) || (Cache == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Status = FindSlabForObject (Cache, *Object, &Slab, &Bit);
    if (FAILED (Status)) {
        goto Exit;
    }

    Status = FreeObjectInSlab (Slab, Flags, Bit);
    if (FAILED (Status)) {
        goto Exit;
    }

Exit:
    return Status;
}

// --------------------------------------------------- Constructor / Destructor

STATUS
SYSAPI
RtlDefaultSlabCtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if ((Object == NULL) || (Cache == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Flags & SLAB_CACHE_FLAGS_ZERO) {
        RtlZeroMemory (Object, Cache->ObjectSize);
    }

Exit:
    return Status;
}

STATUS
SYSAPI
RtlDefaultSlabDtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if ((Object == NULL) || (Cache == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Flags & SLAB_CACHE_FLAGS_ZERO) {
        RtlZeroMemory (Object, Cache->ObjectSize);
    }

Exit:
    return Status;
}
