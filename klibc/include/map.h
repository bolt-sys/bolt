//
// Created by Emil Glenn Karpawitz on 8/27/23.
//

#ifndef BOLT_KLIBC_INCLUDE_MAP_H_
#define BOLT_KLIBC_INCLUDE_MAP_H_

#include "stddef.h"

typedef struct
{
	void* key;
	void* value;
} map_entry;

typedef struct
{
	size_t size;
	size_t capacity;
	map_entry* entries;
} map;

unsigned long hash(void* key, size_t len);

void map_init(map* m, size_t capacity);

void map_put(map* m, void* key, void* value, size_t key_size);

void* map_get(map* m, void* key, size_t key_size);
#endif //BOLT_KLIBC_INCLUDE_MAP_H_
