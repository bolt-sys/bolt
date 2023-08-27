//
// Created by Emil Glenn Karpawitz on 8/27/23.
//
#include "map.h"
#include "stdlib.h"

unsigned long hash(void* key, size_t len)
{
	unsigned long hash = 0xcbf29ce484222325;
	unsigned char *p = (unsigned char *) key;
	for (size_t i = 0; i < len; i++)
	{
		hash ^= p[i];
		hash *= 0x100000001b3;
	}
	return hash;
}

void map_init(map *m, size_t capacity)
{
	m->size = 0;
	m->capacity = capacity;
	m->entries = (map_entry *) malloc(sizeof(map_entry) * capacity);
}