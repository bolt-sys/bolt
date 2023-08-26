//
// Created by Robin Alexander Plate on 8/26/23.
//

#ifndef BOLT_KLIBC_INCLUDE_STDLIB_H_
#define BOLT_KLIBC_INCLUDE_STDLIB_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Implemented in //kernel/source/mem/alloc.c

void* malloc (size_t size);
void* calloc (size_t num, size_t size);
void* realloc (void* ptr, size_t size);
void free (void* ptr);

#ifdef __cplusplus
}
#endif

#endif //BOLT_KLIBC_INCLUDE_STDLIB_H_
