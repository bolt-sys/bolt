#include <RuntimeLib.h>
#include <Kernel.h>

#include "limine.h"

BUMP_ALLOCATOR g_BumpAllocator;

//
// Set the base revision to 1, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.
//

LIMINE_BASE_REVISION (1);

//
// Memory map
//

struct limine_memmap_request g_LimineMemmap = {
    .id       = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

VOID
StartupRoutine (
    VOID
    )
{
    UINTN BumperIdx;

    GDTInit ();
    IDTInit ();

    //
    // Initialize the bump allocator
    //
    BumperIdx = 0;
    for (UINT64 i = 0; i < g_LimineMemmap.response->entry_count; i++) {
        if (g_LimineMemmap.response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            BUMPER* bumper = &g_BumpAllocator.bumpers[BumperIdx++];
            bumper->heap_start = g_LimineMemmap.response->entries[i]->base;
            bumper->heap_end   = g_LimineMemmap.response->entries[i]->base + g_LimineMemmap.response->entries[i]->length;
            bumper->next       = bumper->heap_start;

            if (BumperIdx == MAXIMUM_BUMPERS) {
                // Memory is too fragmented, we can't continue. This is fine but we'll use
                // some usable memory.

                // TODO: log warning
                break;
            }
        }
    }

    RtlBumpAllocatorInitialize (&g_BumpAllocator);

    //
    // Enter the common startup routine
    //
    // This is basically; cross-architecture, cross-platform and cross-bootloader
    // initialization.
    //
    CommonStartupRoutine ((PAGE_ALLOCATOR*)&g_BumpAllocator);
}
