#pragma once
#include "Base.h"
#include "Kernel.h"

typedef struct {
    UINT64 SmallBlockSize;
    UINT64 LargeBlockSize;
    UINT8* BitMaps;
    UINT64 LayerCount;
    UINT64 Layer0BlockCount;
} BuddyAllocator;

EXTERN BuddyAllocator* KernelBuddyAllocator;

STATUS
SYSAPI
KernelBuddyAllocate (
    IN UINT64  Size,
    OUT VOID** Address
    );

STATUS
SYSAPI
KernelBuddyFree (
    IN VOID* Address
    );

STATUS
SYSAPI
KernelBuddyAllocatorInit (
    IN KERNEL_PARAMETERS* Parameters
    );

STATUS
SYSAPI
BuddyAllocatorInit (
    IN OUT BuddyAllocator** Allocator,
    IN KERNEL_PARAMETERS*   Parameters
    );

STATUS
SYSAPI
BuddyAllocate (
    IN BuddyAllocator* Allocator,
    IN UINT64          Size,
    OUT VOID**         Address
    );

STATUS
SYSAPI
BuddyFree (
    IN OUT BuddyAllocator* Allocator,
    IN     VOID*           Address
    );
