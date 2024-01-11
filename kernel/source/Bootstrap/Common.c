#include <RuntimeLib.h>
#include <Kernel.h>

PAGE_ALLOCATOR* g_PageAllocator;

VOID
CommonStartupRoutine (
    PAGE_ALLOCATOR* PageAllocator
    )
{
    g_PageAllocator = PageAllocator;

    KernelMain ();
}

// TODO: General purpose memory allocation functions using slab allocator
