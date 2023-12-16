#include <RuntimeLib.h>
#include <Kernel.h>

PAGE_ALLOCATOR* g_PageAllocator;

VOID
CommonStartupRoutine (
    PAGE_ALLOCATOR* BumpAllocator
    )
{
    // TODO: g_PageAllocator = BuddyAllocatorInit (BumpAllocator);
    g_PageAllocator = BumpAllocator;

    KernelMain ();
}

// TODO: General purpose memory allocation functions using slab allocator
