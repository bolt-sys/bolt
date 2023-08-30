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

    // we need to make sure that the freemap is filled with 1's
    // so that we know which are free and which are reserved.
    FillMemory (Slab->FreeMap, Slab->FreeMapEnd - Slab->FreeMap, 0xFF);

    // Mark all objects as free
    for (UINTN i = 0; i < Cache->ObjectCount / 8; i++) {
        Slab->FreeMap[i] = 0;
    }

    Slab->Cache  = Cache;
    Slab->Next   = Cache->Empty;
    Cache->Empty = Slab;

Exit:
    return Status;
}
