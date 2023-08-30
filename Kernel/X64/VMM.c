#include "VirtualMemory.h"

#include <Base.h>

typedef UINTN PT[512];
typedef PT*   PD[512];
typedef PD*   PDPT[512];
typedef PDPT* PML4[512];
// NOTE: this is highly architecture dependent,
//       some CPUs don't support 5-level paging
typedef PML4* PML5[512];

typedef struct {
    UINT64 Reserved;
} PACKED MEMORY_DESCRIPTOR;

STATIC_ASSERT (sizeof (MEMORY_DESCRIPTOR) == 8, "Invalid size for MEMORY_DESCRIPTOR");

typedef struct {
    PML5*              PML5; // NULL if not supported
    PML4*              PML4;

    UINTN              MemoryDescriptorCount;
    MEMORY_DESCRIPTOR* MemoryDescriptors;
} MEMORY_MAP;

/**
 * @brief Create a new Memory Map, used to control the Memory Layout of the System,
 *        it usually is also referred to as the Virtual Memory Manager.
 *
 *        The memory will be automatically claimed and will be used to store the
 *        Page tables and the Page Directory.
 *
 * @param[out] MemoryMap              The Memory Map
 *
 * @return STATUS_SUCCESS             The Memory Map was created successfully
 **/
STATUS
SYSAPI
CreateMemoryMap (
    OUT HMEMORY_MAP* MemoryMap
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

/**
 * @brief Destroy a Memory Map
 *
 * @param[in,out] MemoryMap The Memory Map to destroy
 *
 * @return
 **/
STATUS
SYSAPI
DestroyMemoryMap (
    IN OUT HMEMORY_MAP* MemoryMap
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

/**
 * @brief Create a new Memory Descriptor
 *        This function creates a new Memory Descriptor, which can be used to
 *        describe a region of memory.
 *
 *        The Memory Descriptor can be added to a Memory Map using the
 *        AddMemoryDescriptor function.
 *
 * @param[in]          SourceAddress      The physical address of the memory region
 * @param[in]          DestinationAddress The virtual address of the memory region
 * @param[in]          NumberOfPages      Number of pages to describe
 * @param[in,optional] Flags              Flags for the Memory Descriptor
 * @param[out]         MemoryDescriptor   The Memory Descriptor
 *
 * @return STATUS_SUCCESS             The Memory Descriptor was created successfully
 *
 * @return STATUS_INVALID_PARAMETER   SourceAddress or DestinationAddress are NULL
 *                                    NULL is a reserved address and cannot be used
 *
 * @return STATUS_INVALID_PARAMETER   NumberOfPages is 0
 *
 * @return STATUS_INVALID_PARAMETER   MemoryDescriptor is NULL
 *
 * @return STATUS_INVALID_PARAMETER   The Memory Descriptor is invalid
 *
 * @return STATUS_OVERLAPPING_BUFFER  The Memory Descriptor overlaps with another
 *                                    Memory Descriptor which will cause a conflict.
 *
 *                                    This can happen if the Memory Descriptor is
 *                                    already added to a Memory Map.
 *
 * @return STATUS_UNALIGNED           SourceAddress or DestinationAddress are not
 *                                    aligned to the page size
 *                                    (PAGE_SIZE_4K, PAGE_SIZE_2M, PAGE_SIZE_1G)
 *
 * @return STATUS_OUT_OF_MEMORY       The Memory Descriptor could not be created
 *                                    due to lack of memory
 */
STATUS
SYSAPI
CreateMemoryDescriptor (
    IN           PHYSICAL_ADDRESS    SourceAddress,
    IN           VIRTUAL_ADDRESS     DestinationAddress,
    IN           UINTN               NumberOfPages,
    IN  OPTIONAL UINTN               Flags,
    IN  OPTIONAL UINTN               Type,
    OUT          HMEMORY_DESCRIPTOR* MemoryDescriptor
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

/**
 * @brief Destroy a Memory Descriptor
 *
 * @param[in,out] MemoryDescriptor    The Memory Descriptor to destroy
 *
 * @return STATUS_SUCCESS             The Memory Descriptor was destroyed successfully
 */
STATUS
SYSAPI
DestroyMemoryDescriptor (
    IN OUT HMEMORY_DESCRIPTOR* MemoryDescriptor
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

/**
 * @brief Add a Memory Descriptor to a Memory Map
 *
 * @param[in] MemoryDescriptor   The Memory Descriptor to add
 * @param[in] MemoryMap          The Memory Map to add the Memory Descriptor to
 *
 * @return STATUS_SUCCESS        The Memory Descriptor was added successfully
 */
STATUS
SYSAPI
AddMemoryDescriptor (
    IN HMEMORY_DESCRIPTOR MemoryDescriptor,
    IN HMEMORY_MAP        MemoryMap
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

/**
 * @brief Remove a Memory Descriptor from a Memory Map
 *
 * @param MemoryDescriptor The Memory Descriptor to remove
 *
 * @return STATUS_SUCCESS  The Memory Descriptor was removed successfully
 */
STATUS
SYSAPI
RemoveMemoryDescriptor (
    IN HMEMORY_DESCRIPTOR MemoryDescriptor
    )
{
    STATUS Status;

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}
