//
// Created by Robin Alexander Plate on 8/25/23.
//

#ifndef BOLT_KLIBC_STRING_H_
#define BOLT_KLIBC_STRING_H_

#include <stddef.h>

#ifdef __cplusplus
#define restrict // C++ doesn't have restrict
extern "C" {
#endif

/**
 * @brief Copies count bytes from src to dest
 * @param dest The destination pointer
 * @param src The source pointer
 * @param count The number of bytes to copy
 * @return The destination pointer.
 */
void *memcpy(void *restrict dest, const void *restrict src, size_t count);

/**
 * @brief Copies count bytes from src to dest, even if the memory regions overlap
 * @param dest The destination pointer
 * @param src The source pointer
 * @param count The number of bytes to copy
 * @return The destination pointer.
 */
void *memmove(void *dest, const void *src, size_t count);

/**
 * @brief Sets count bytes in dest to ch
 * @param dest The destination pointer
 * @param ch The byte to set
 * @param count The number of bytes to set
 * @return The destination pointer.
 */
void *memset(void *dest, int ch, size_t count);

/**
 * @brief Compares count bytes in lhs and rhs
 * @param lhs The first memory region
 * @param rhs The second memory region
 * @param count The number of bytes to compare
 * @return An integer less than, equal to, or greater than zero if lhs is found, respectively,
 *         to be less than, to match, or be greater than rhs.
 */
int memcmp(const void *lhs, const void *rhs, size_t count);

#ifdef __cplusplus
}
#undef restrict
#endif

#endif