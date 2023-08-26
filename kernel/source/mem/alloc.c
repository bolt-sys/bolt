#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bump_allocator.h"

#define MEMORY_BLOCK_COOKIE 0x12345678

typedef struct MemoryBlock {
  size_t cookie;
  size_t size;
  size_t capacity;
  uint8_t data[0]; // data[size]
} MemoryBlock;

void *malloc(size_t size) {
  MemoryBlock *block = BumpAllocator_alloc(size + sizeof(MemoryBlock));
  if (block == NULL) {
	return NULL;
  }

  block->cookie = MEMORY_BLOCK_COOKIE;
  block->size = size;
  block->capacity = size;

  return block->data;
}

void *calloc(size_t num, size_t size) {
  size_t total_size = num * size;
  void *ptr = malloc(total_size);
  memset(ptr, 0, total_size);
  return ptr;
}

void *realloc(void *ptr, size_t size) {
  if (ptr == NULL) {
	return malloc(size); // Allocate a new block.
  }

  MemoryBlock *block = (MemoryBlock *)ptr - 1;
  if (block->cookie != MEMORY_BLOCK_COOKIE) {
	return NULL; // Invalid pointer.
  }

  if (size <= block->capacity) {
	block->size = size; // Shrink the block.
	return ptr;
  }

  void *new_ptr = malloc(size);
  if (new_ptr == NULL) {
	return NULL; // Out of memory.
  }

  memcpy(new_ptr, ptr, block->size);
  free(ptr);
  return new_ptr;
}

void free(void *ptr) {
  // Not implemented in the bump allocator.
  // so this is a no-op.
}
