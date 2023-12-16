#pragma once

#ifndef __KERNEL_H__
    #error "This file should be included from Kernel.h"
#endif

VOID
CommonStartupRoutine(
    PAGE_ALLOCATOR *BumpAllocator
    );
