#include <Memory.h>
#include <Kernel.h>

//
// TODO: Replace this with a buddy allocator
//

typedef struct {
    UINTN heap_start;
    UINTN heap_end;
    UINTN next;
} BUMPER;

typedef struct {
    BUMPER bumpers[512];
} BUMP_ALLOCATOR;

STATIC BUMP_ALLOCATOR g_BumpAllocator;
STATIC BOOLEAN        g_BumpAllocator_initialized = FALSE;

/**
 * @brief Initialize the Bump Allocator
 *
 * @return STATUS_SUCCESS             The initialization was successful
 * @return STATUS_ALREADY_INITIALIZED The Bump Allocator is already initialized
 * @return STATUS_OUT_OF_RESOURCES    The Bump Allocator could not be initialized
 *                                                                    due to lack of memory
 */
STATUS
SYSAPI
MemoryInit (
    KERNEL_PARAMETERS* Parameters
    )
{
    STATUS             Status;
    INTN               Bumpers;
    MEMORY_DESCRIPTOR* Descriptor;

    Status = STATUS_SUCCESS;

    if (g_BumpAllocator_initialized) {
        Status = STATUS_ALREADY_INITIALIZED;
        goto Exit;
    }

    ZeroMemory (&g_BumpAllocator, sizeof (BUMP_ALLOCATOR));

    Bumpers = 0;

    for (UINTN i = 0; i < Parameters->MemoryMapSize; i++) {
		Descriptor = Parameters->MemoryMap[i];
        if (Descriptor->Type != MEMMAP_USABLE) {
            continue;
        }

        g_BumpAllocator.bumpers[Bumpers].heap_start = Descriptor->Base;
        g_BumpAllocator.bumpers[Bumpers++].heap_end = Descriptor->Base + Descriptor->Length;

        if (Bumpers >= 512) {
            return STATUS_OUT_OF_RESOURCES; // TOO MANY BUMPERS!
        }
    }

    g_BumpAllocator_initialized = TRUE;

Exit:
    return Status;
}

/**
 * @brief Allocate a range of pages
 *
 * @param[out] Address The address of the allocated memory
 * @param[in]  Pages   The amount of pages to allocate
 * @param[in]  Align   The alignment of the allocation
 *                     must be (PAGE_SIZE_4K, PAGE_SIZE_2M, PAGE_SIZE_1G)
 *
 * @return STATUS_SUCCESS             The allocation was successful
 * @return STATUS_INVALID_PARAMETER   Address is NULL or Pages is 0
 * @return STATUS_UNALIGNED           Align is not a valid page size
 * @return STATUS_OUT_OF_MEMORY       The allocation failed
 * @return STATUS_NOT_INITIALIZED     The Allocator is not initialized
 *
 * @note This function must be implemented by the calling api.
 *       It is not implemented in the RuntimeLib.
 *
 *       it is being used by the RTL Memory Management functions.
 **/
STATUS
SYSAPI
AllocatePages (
    OUT VOID** Address,
    IN  UINTN  Pages,
    IN  UINTN  Align
    )
{
    STATUS Status;
    UINTN  Size;

    Status = STATUS_SUCCESS;

    if (!g_BumpAllocator_initialized) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if ((Address == NULL) || (Pages == 0)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if ((Align != PAGE_SIZE_4K) && (Align != PAGE_SIZE_2M) && (Align != PAGE_SIZE_1G)) {
        Status = STATUS_UNALIGNED;
        goto Exit;
    }

    Size = Pages * Align;

    for (BUMPER* bumper = g_BumpAllocator.bumpers; bumper->heap_start != 0; bumper++) {
        if (bumper->next + Size <= bumper->heap_end) {
            *Address      = (void*)bumper->next + bumper->heap_start;
            bumper->next += Size;
            goto Exit;
        }
    }

    Status = STATUS_OUT_OF_MEMORY;

Exit:
    if (Status != STATUS_SUCCESS) {
        *Address = NULL;
    }

    return Status;
}

/**
 * @brief Free a range of pages
 *
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Pages   The amount of pages to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL or Pages is 0
 * @return STATUS_UNALIGNED            The address is not page aligned
 * @return STATUS_NOT_IMPLEMENTED      The function is not implemented
 * @return STATUS_NOT_INITIALIZED      The Allocator is not initialized
 *
 * @note This function will not check for the validity of the address
 *       or the amount of pages to free. It is up to the caller to
 *       ensure that the address and the amount of pages are correct.
 *
 * @note This function must be implemented by the calling api.
 *       It is not implemented in the RuntimeLib.
 *
 *       it is being used by the RTL Memory Management functions.
 **/
STATUS
SYSAPI
FreePages (
    IN OUT VOID** Address,
    IN     UINTN  Pages
    )
{
    if ((Address == NULL) || (Pages == 0)) {
        return STATUS_INVALID_PARAMETER;
    }

    *Address = NULL;

    return STATUS_NOT_IMPLEMENTED;     // Not implemented for the bump allocator
}
