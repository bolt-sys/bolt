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

/**
 * @brief Copies the string src to dest (including the terminating '\0' character.)
 * @param dest The destination pointer
 * @param src The source pointer
 * @return The destination pointer.
 */
char* strcpy(char *dest, const char *src);

/**
 * @brief Copies up to n characters from the string src to dest.
 * @param dest The destination pointer
 * @param src The source pointer
 * @param n The maximum number of characters to copy
 * @return The destination pointer.
 */
char *strncpy(char* dest, const char *src, size_t n);

/**
 * @brief Copies a string to a destination buffer with a specified maximum size.
 *
 * This function copies the source string 'src' to the destination buffer 'dest',
 * ensuring that the copied string is null-terminated, even if the source string is
 * longer than the specified maximum size 'n-1'.
 *
 * @param dest The destination buffer where the copied string will be stored.
 * @param src The source string to be copied.
 * @param n The maximum size of the destination buffer, including the null
 */
char *strlcpy(char* dest, const char *src, size_t n);

/**
 * @brief Checks if the string is lowercase
 * @param c The string to check
 * @return 1 if the string is lowercase, 0 otherwise
 */
int	 str_is_lowercase(char *c);

/**
 * @brief Checks if the string is uppercase
 * @param c The string to check
 * @return 1 if the string is uppercase, 0 otherwise
 */
int	 str_is_uppercase(char *c);

/**
 * @brief Checks if the string is numeric
 * @param c The string to check
 * @return 1 if the string is numeric, 0 otherwise
 */
int	 str_is_numeric(char *c);

/**
 * @brief Converts a string to uppercase string
 * @param s The string to convert
 * @return The uppercase string
 * */
char* strupr(char* s);

/**
 * @brief Converts a string to lowercase string
 * @param s The string to convert
 * @return The lowercase string
 * */
char* strlwr(char* s);

/**
 * @brief Capitalizes a string
 * @param s The string to capitalize
 * @return The capitalized string
 */
char* strcapitalize(char* s);

int strcmp(const char *lhs, const char *rhs);



#ifdef __cplusplus
}
#undef restrict
#endif

#endif