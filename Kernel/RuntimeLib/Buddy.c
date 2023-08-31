#include "InternalMemory.h"
#include "Mathi.h"
#include "Memory.h"

BuddyAllocator* KernelBuddyAllocator;

STATIC
STATUS
SYSAPI
CalculateMemorySize (
    IN KERNEL_PARAMETERS* Parameters,
    OUT UINT64*           Size
    )
{
    for (UINTN I = 0; I < Parameters->MemoryMapSize; ++I) {
        MEMORY_DESCRIPTOR* Descriptor = Parameters->MemoryMap[I];
        if (Descriptor->Type == MEMMAP_USABLE) {
            *Size += Descriptor->Length;
        }
    }

    return STATUS_SUCCESS;
}

STATIC
STATUS
SYSAPI
BiggestBlockSize (
    IN UINT64   MemorySize,
    OUT UINT64* BlockSize
    )
{
    *BlockSize = Pow2I (Log2I (MemorySize));
    return STATUS_SUCCESS;
}

STATIC
STATUS
SYSAPI
CalculateLayerCount (
    IN UINT64   SmallBlockSize,
    IN UINT64   LargeBlockSize,
    OUT UINT64* LayerCount
    )
{
    *LayerCount = 0;
    while (SmallBlockSize != LargeBlockSize) {
        SmallBlockSize *= 2;
        ++*LayerCount;
    }

    return STATUS_SUCCESS;
}

STATUS
SYSAPI
BuddyAllocatorInit (
    IN OUT BuddyAllocator** AllocatorPtr,
    IN KERNEL_PARAMETERS*   Parameters
    )
{
    UINT64          MemorySize = 0;
    BuddyAllocator* Allocator  = *AllocatorPtr;
    STATUS          Status     = STATUS_SUCCESS;

    CalculateMemorySize (Parameters, &MemorySize);

    Allocator->SmallBlockSize = 0x1000;
    BiggestBlockSize (MemorySize, &Allocator->LargeBlockSize);
    if (Allocator->LargeBlockSize < Allocator->SmallBlockSize) {
        Status = STATUS_OUT_OF_MEMORY;
        goto Exit;
    }

    Allocator->Layer0BlockCount = DivideRoundUpI (MemorySize, Allocator->LargeBlockSize);
    CalculateLayerCount (Allocator->SmallBlockSize, Allocator->LargeBlockSize, &Allocator->LayerCount);
    AllocatePages (
      (void**)&Allocator->BitMaps,
      MaxI (Allocator->LayerCount * Allocator->Layer0BlockCount / 8 / 4096, 1),
      PAGE_SIZE_4K
      );
    ZeroMemory (Allocator->BitMaps, Allocator->LayerCount * Allocator->Layer0BlockCount / 8);
Exit:
    if (Status != STATUS_SUCCESS) {
        *AllocatorPtr = NULL;
    }

    return Status;
}

STATUS
SYSAPI
KernelBuddyAllocatorInit (
    IN KERNEL_PARAMETERS* Parameters
    )
{
    AllocatePages ((void**)&KernelBuddyAllocator, 1, PAGE_SIZE_4K);
    return BuddyAllocatorInit (&KernelBuddyAllocator, Parameters);
}

STATUS
SYSAPI
BuddyAllocate (
    IN BuddyAllocator* Allocator,
    IN UINT64          Size,
    OUT VOID**         Address
    )
{
    STATUS Status = STATUS_SUCCESS;

    if (Allocator->LargeBlockSize < Size) {
        Status = STATUS_OUT_OF_MEMORY;
        goto Exit;
    }

    UINT64 RequiredBlockSize = MaxI (Allocator->SmallBlockSize, Size);
    UINT64 Layer             = Log2I (Allocator->LargeBlockSize) - Log2I (RequiredBlockSize);
    UINT64 BlockMapIndex     = 0;

    for ( ; Layer < Allocator->LayerCount; ++Layer) {
        UINT64 LayerBlockCount = Allocator->Layer0BlockCount >> Layer;
        UINT64 LayerBlockSize  = Allocator->LargeBlockSize >> Layer;
        UINT64 LayerBlockIndex = 0;
        for ( ; LayerBlockIndex < LayerBlockCount; ++LayerBlockIndex) {
            UINT64 BlockMapByteIndex = BlockMapIndex / 8;
            UINT64 BlockMapBitIndex  = BlockMapIndex % 8;
            UINT8  BlockMapByte      = Allocator->BitMaps[BlockMapByteIndex];
            if ((BlockMapByte & (1 << BlockMapBitIndex)) == 0) {
                Allocator->BitMaps[BlockMapByteIndex] |= (1 << BlockMapBitIndex);
                *Address                               = (VOID*)(LayerBlockIndex * LayerBlockSize);
                goto Exit;
            }

            ++BlockMapIndex;
        }
    }

Exit:
    if (Status != STATUS_SUCCESS) {
        *Address = NULL;
    }

    return Status;
}

STATUS
SYSAPI
KernelBuddyAllocate (
    IN UINT64  Size,
    OUT VOID** Address
    )
{
    return BuddyAllocate (KernelBuddyAllocator, Size, Address);
}

STATUS
SYSAPI
BuddyFree (
    IN OUT BuddyAllocator* Allocator,
    IN     VOID*           Address
    )
{
    STATUS Status          = STATUS_SUCCESS;
    UINT64 Layer           = 0;
    UINT64 LayerBlockIndex = (UINT64)Address / Allocator->LargeBlockSize;
    UINT64 BlockMapIndex   = LayerBlockIndex;

    for ( ; Layer < Allocator->LayerCount; ++Layer) {
        UINT64 LayerBlockIndex    = BlockMapIndex / 8;
        UINT64 LayerBlockBitIndex = BlockMapIndex % 8;
        UINT8  BlockMapByte       = Allocator->BitMaps[LayerBlockIndex];
        if ((BlockMapByte & (1 << LayerBlockBitIndex)) == 0) {
            Status = STATUS_INVALID_PARAMETER;
            goto Exit;
        }

        Allocator->BitMaps[LayerBlockIndex] &= ~(1 << LayerBlockBitIndex);
        BlockMapIndex                       /= 2;
    }

Exit:
    return Status;
}

STATUS
SYSAPI
KernelBuddyFree (
    IN VOID* Address
    )
{
    return BuddyFree (KernelBuddyAllocator, Address);
}
