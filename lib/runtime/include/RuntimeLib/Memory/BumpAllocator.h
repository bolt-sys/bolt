#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

typedef struct {
    UINTN heap_start;
    UINTN heap_end;
    UINTN next;
} BUMPER;

typedef struct {
    BUMPER bumpers[512];
} BUMP_ALLOCATOR;

/**
 * @brief Allocate a range of pages
 *
 * @param[out] Address       The address of the allocated memory
 * @param[in]  BumpAllocator The allocator to use
 * @param[in]  Pages         The amount of pages to allocate
 * @param[in]  Align         The alignment of the allocation
 *                           must be (PAGE_SIZE_4K, PAGE_SIZE_2M, PAGE_SIZE_1G)
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
    );
