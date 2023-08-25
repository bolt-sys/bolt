#include <string.h>

/**
 * @brief Copies count bytes from src to dest
 * @param dest The destination pointer
 * @param src The source pointer
 * @param count The number of bytes to copy
 * @return The destination pointer.
 */
void *memcpy(void *restrict dest, const void *restrict src, size_t count) {
  char *dest8 = (char *)dest;
  const char *src8 = (const char *)src;

  while (count--) {
	*dest8++ = *src8++;
  }

  return dest;
}

/**
 * @brief Copies count bytes from src to dest, even if the memory regions overlap
 * @param dest The destination pointer
 * @param src The source pointer
 * @param count The number of bytes to copy
 * @return The destination pointer.
 */
void *memmove(void *dest, const void *src, size_t count) {
  char *dest8 = (char *)dest;
  const char *src8 = (const char *)src;

  if (dest8 < src8) {
	while (count--) {
	  *dest8++ = *src8++;
	}
  } else {
	dest8 += count;
	src8 += count;
	while (count--) {
	  *--dest8 = *--src8;
	}
  }

  return dest;
}

/**
 * @brief Sets count bytes in dest to ch
 * @param dest The destination pointer
 * @param ch The byte to set
 * @param count The number of bytes to set
 * @return The destination pointer.
 */
void *memset(void *dest, int ch, size_t count) {
  char *dest8 = (char *)dest;
  char ch8 = (char)ch;

  while (count--) {
	*dest8++ = ch8;
  }

  return dest;
}

/**
 * @brief Compares count bytes in lhs and rhs
 * @param lhs The first memory region
 * @param rhs The second memory region
 * @param count The number of bytes to compare
 * @return An integer less than, equal to, or greater than zero if lhs is found, respectively,
 *         to be less than, to match, or be greater than rhs.
 */
int memcmp(const void *lhs, const void *rhs, size_t count) {
  const char *lhs8 = (const char *)lhs;
  const char *rhs8 = (const char *)rhs;

  while (count--) {
	if (*lhs8 != *rhs8) {
	  return *lhs8 - *rhs8;
	}

	lhs8++;
	rhs8++;
  }

  return 0;
}
