//
// Created by Robin Alexander Plate on 8/25/23.
//

#include "bump_allocator.h"

#include <string.h>
#include <stdbool.h>
#include <limine.h>

static volatile struct limine_memmap_request
	memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static BumpAllocator g_BumpAllocator;
static bool g_BumpAllocator_initialized = false;

/**
 * @brief Initialize a bump allocator
 * @note This function must only be called ONCE!!!
 *       do not call it again, even if you create a new allocator.
 *
 *       This is due to the fact that the bump allocator is using all of
 *       the systems memory, and it will not be able to allocate any more
 *       memory for itself.
 */
void BumpAllocator_init()
{
	if (g_BumpAllocator_initialized) {
		return;
	}

	memset(&g_BumpAllocator, 0, sizeof(BumpAllocator));

	int bumpers = 0;

	for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
		struct limine_memmap_entry* entry = memmap_request.response->entries[i];
		if (entry->type != LIMINE_MEMMAP_USABLE) {
			continue;
		}

		g_BumpAllocator.bumpers[bumpers].heap_start = entry->base;
		g_BumpAllocator.bumpers[bumpers++].heap_end = entry->base + entry->length;

		if (bumpers >= 512) {
			// TODO: raise error
			break; // TOO MANY BUMPERS!
		}
	}

	g_BumpAllocator_initialized = true;
}

/**
 * @brief Allocate memory from a bump allocator
 * @param allocator The allocator to allocate from
 * @param size The size of the allocation
 * @return The address of the allocation, or 0 if no memory is available
 */
void* BumpAllocator_alloc(size_t size)
{
	if (!g_BumpAllocator_initialized) {
		return NULL;
	}

	for (Bumper* bumper = g_BumpAllocator.bumpers; bumper->heap_start != 0; bumper++) {
		if (bumper->next + size <= bumper->heap_end) {
			void* ptr = (void*)bumper->next + bumper->heap_start;
			bumper->next += size;
			return ptr;
		}
	}

	return NULL; // No memory available
}
