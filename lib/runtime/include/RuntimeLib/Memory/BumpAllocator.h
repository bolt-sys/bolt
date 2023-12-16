#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

#define MAXIMUM_BUMPERS 512

typedef struct {
    UINTN heap_start;
    UINTN heap_end;
    UINTN next;
} BUMPER;

typedef struct {
    // Inherited from PAGE_ALLOCATOR
    PA_ALLOCATE_PAGES AllocatePages;
    PA_FREE_PAGES     FreePages;

    // Bump allocator specific
    BUMPER            bumpers[MAXIMUM_BUMPERS];
} BUMP_ALLOCATOR;

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
    );
