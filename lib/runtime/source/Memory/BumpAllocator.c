#include <RuntimeLib.h>

STATUS
SYSAPI
BumpAllocator_AllocatePages (
    IN BUMP_ALLOCATOR* Self,
    OUT VOID**         Address,
    IN  UINTN          Pages,
    IN  UINTN          Align
    );

STATUS
SYSAPI
BumpAllocator_FreePages (
    IN     BUMP_ALLOCATOR* Self,
    IN OUT VOID**          Address,
    IN     UINTN           Pages
    );

/**
 * @brief Empty Initialize an bump allocator.
 *
 *        This will initialize the core structure of the bump allocator,
 *        like setting up the vtable or any other necessary structs.
 *        However, it will not populate the inner bumpers and is still required by the callee.
 *
 *  @param[in] BumpAllocator The allocator to initialize
 *
 *  @return STATUS_SUCCESS             The initialization was successful
 *  @return STATUS_INVALID_PARAMETER   BumpAllocator is NULL
 */
STATUS
SYSAPI
RtlBumpAllocatorInitialize (
    IN OUT BUMP_ALLOCATOR* BumpAllocator
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if (BumpAllocator == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    BumpAllocator->AllocatePages = (PA_ALLOCATE_PAGES)BumpAllocator_AllocatePages;
    BumpAllocator->FreePages     = (PA_FREE_PAGES)BumpAllocator_FreePages;

Exit:
    return Status;
}

/**
 * @brief Allocate a range of pages
 *
 * @param[in]  Self    The allocator to use
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
BumpAllocator_AllocatePages (
    IN BUMP_ALLOCATOR* Self,
    OUT VOID**         Address,
    IN  UINTN          Pages,
    IN  UINTN          Align
    )
{
    STATUS Status;
    UINTN  Size;

    Status = STATUS_SUCCESS;

    if (Self == NULL) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if ((Address == NULL) || (Pages == 0)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if ((Align != PAGE_SIZE_4K) && (Align != PAGE_SIZE_2M) && (Align != PAGE_SIZE_1G)) {
        Status = STATUS_INVALID_ALIGNMENT;
        goto Exit;
    }

    *Address = NULL;

    Size = Pages * Align;

    for (BUMPER* bumper = Self->bumpers; bumper->heap_start != 0; bumper++) {
        UINTN Next    = bumper->next + Size;
        UINTN HeapEnd = bumper->heap_end;

        if (Next <= HeapEnd) {
            *Address     = (void*)bumper->next;
            bumper->next = Next;
            goto Exit;
        }
    }

    Status = STATUS_OUT_OF_MEMORY;

Exit:
    if ((Status != STATUS_SUCCESS) && (Address != NULL)) {
        *Address = NULL;
    }

    return Status;
}

/**
 * @brief Free a range of pages
 *
 * after the page has been freed, *Address will be set to NULL.
 *
 * @param[in]     Self    The allocator to use
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Pages   The amount of pages to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL or Pages is 0
 * @return STATUS_INVALID_ALIGNMENT    The address is not page aligned
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
BumpAllocator_FreePages (
    IN     BUMP_ALLOCATOR* Self,
    IN OUT VOID**          Address,
    IN     UINTN           Pages
    )
{
    STATUS Status;

    UNUSED (Address);
    UNUSED (Pages);

    Status = STATUS_SUCCESS;

    if (Self == NULL) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if ((Address == NULL) || (Pages == 0)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    *Address = NULL;

    // We cannot free pages, thus we leak.
    // TODO: print a warning somehow? maybe an ASSERT?

Exit:
    return Status;
}
