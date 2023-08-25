//
// Created by Robin Alexander Plate on 8/25/23.
//

#include "bump_allocator.h"

#include <string.h>
#include <limine.h>

static volatile struct limine_memmap_request
	memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

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
void BumpAllocator_init(BumpAllocator *allocator) {
  memset(allocator, 0, sizeof(BumpAllocator));

  int bumpers = 0;

  for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
	struct limine_memmap_entry *entry = memmap_request.response->entries[i];
	if (entry->type != LIMINE_MEMMAP_USABLE) {
	  continue;
	}

	allocator->bumpers[bumpers].heap_start = entry->base;
	allocator->bumpers[bumpers++].heap_end = entry->base + entry->length;

	if (bumpers >= 512) {
	  // TODO: raise error
	  break; // TOO MANY BUMPERS!
	}
  }
}

/**
 * @brief Allocate memory from a bump allocator
 * @param allocator The allocator to allocate from
 * @param size The size of the allocation
 * @return The address of the allocation, or 0 if no memory is available
 */
void *BumpAllocator_alloc(BumpAllocator *allocator, size_t size) {
  for (Bumper *bumper = allocator->bumpers; bumper->heap_start != 0; bumper++) {
	if (bumper->next + size <= bumper->heap_end) {
	  void *ptr = (void *)bumper->next + bumper->heap_start;
	  bumper->next += size;
	  return ptr;
	}
  }

  return NULL; // No memory available
}
