//
// Created by robin on 8/25/23.
//

#ifndef BOLT_KERNEL_SOURCE_MEM_BUMP_ALLOCATOR_H_
#define BOLT_KERNEL_SOURCE_MEM_BUMP_ALLOCATOR_H_

#include <stddef.h>

typedef struct Bumper {
  size_t heap_start;
  size_t heap_end;
  size_t next;
} Bumper;

typedef struct BumpAllocator {
  Bumper bumpers [512];
} BumpAllocator;

/**
 * @brief Initialize a bump allocator
 * @param allocator The allocator to initialize
 * @note This function must only be called ONCE!!!
 *       do not call it again, even if you create a new allocator.
 *
 *       This is due to the fact that the bump allocator is using all of
 *       the systems memory, and it will not be able to allocate any more
 *       memory for itself.
 */
void BumpAllocator_init(BumpAllocator *allocator);

/**
 * @brief Allocate memory from a bump allocator
 * @param allocator The allocator to allocate from
 * @param size The size of the allocation
 * @return The address of the allocation, or 0 if no memory is available
 */
void *BumpAllocator_alloc(BumpAllocator *allocator, size_t size);

#endif //BOLT_KERNEL_SOURCE_MEM_BUMP_ALLOCATOR_H_
