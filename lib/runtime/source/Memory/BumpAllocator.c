#include <RuntimeLib.h>

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
 * @return STATUS_INVALID_ALIGNMENT   Align is not a valid page size
 * @return STATUS_OUT_OF_MEMORY       The allocation failed
 * @return STATUS_NOT_INITIALIZED     The Allocator is not initialized
 **/
STATUS
SYSAPI
BumpAllocatorAllocatePages (
    OUT VOID**         Address,
    IN BUMP_ALLOCATOR* BumpAllocator,
    IN  UINTN          Pages,
    IN  UINTN          Align
    )
{
    STATUS Status;
    UINTN  Size;

    Status = STATUS_SUCCESS;

    if ((Address == NULL) || (Pages == 0) || (BumpAllocator == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if ((Align != PAGE_SIZE_4K) && (Align != PAGE_SIZE_2M) && (Align != PAGE_SIZE_1G)) {
        Status = STATUS_INVALID_ALIGNMENT;
        goto Exit;
    }

    *Address = NULL;

    Size = Pages * Align;

    for (BUMPER* bumper = BumpAllocator->bumpers; bumper->heap_start != 0; bumper++) {
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
