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
 * @brief Copies 'count' bytes from 'src' to 'dest'; assumes that 'src' and 'dest' do not overlap.
 *
 * This function copies 'count' bytes from 'src' to 'dest'. Keep in mind this function does not
 * check for any possible overlap in memory between 'src' and 'dest', and if 'src' and 'dest'
 * do overlap, the behavior is undefined. Therefore, this function tends to work best when you
 * are sure that the 'src' and 'dest' do not overlap.
 *
 * @param dest The destination buffer, where the content is to be copied.
 * @param src The source of data to be copied.
 * @param count The number of bytes to copy.
 * @return The destination is returned.
 */
void *memcpy(void *restrict dest, const void *restrict src, size_t count);

/**
 * @brief Copies 'count' bytes from 'src' to 'dest', but handles overlapping source and destination.
 *
 * This function copies 'count' bytes from 'src' to 'dest'. If 'src' and 'dest' overlap,
 * the function is still able to handle the situation correctly by copying the characters
 * in a way that the original source characters do not get overwritten.
 *
 * @param dest The destination buffer, where the content is to be copied.
 * @param src The source of data to be copied.
 * @param count The number of bytes to copy.
 * @return The destination is returned.
 */
void *memmove(void *dest, const void *src, size_t count);

/**
 * @brief Sets the first 'count' bytes of the block of memory pointed by 'dest' to the specified value 'ch'.
 *
 * This function fills a block of memory, starting at the location pointed to by 'dest', with the constant byte 'ch' for 'count' bytes.
 * The 'ch' integer value is converted to unsigned char when written to 'dest'.
 *
 * @param dest Pointer to the destination block of memory that will be filled.
 * @param ch The constant byte to set.
 * @param count The number of bytes to set to the value.
 * @return 'dest' is returned.
 */
void *memset(void *dest, int ch, size_t count);

/**
 * @brief Compares two blocks of memory.
 *
 * This function compares the first 'count' bytes of the memory areas 'lhs' and 'rhs'.
 *
 * @param lhs The first memory area to be compared.
 * @param rhs The second memory area to be compared.
 * @param count The maximum number of bytes to compare.
 * @return An integer less than, equal to, or greater than zero if the first 'count' bytes of 'lhs' is found to be less than, matches, or is greater than the first 'count' bytes of 'rhs' respectively.
 */
int memcmp(const void *lhs, const void *rhs, size_t count);

/**
 * @brief Copies a string to a destination buffer.
 *
 * This function copies the source string 'src' to the destination buffer 'dest'.
 * It continues to copy characters from 'src' to 'dest' until encountering a null byte ('\0').
 * As a result, the destination string will be null-terminated.
 * Note : It does not check the size of the destination buffer, 'dest'. This can lead to buffer overflow if 'dest' isn't large enough.
 *
 * @param dest The destination buffer where the copied string will be stored.
 * @param src The source string to be copied.
 * @return The destination buffer.
 */
char* strcpy(char *dest, const char *src);

/**
 * @brief Copies a string to a destination buffer with a specified maximum size.
 *
 * This function copies the source string 'src' to the destination buffer 'dest'.
 * It copies 'n' characters from the source string to the destination buffer. If the 'src' is shorter
 * than 'n', remaining characters in the buffer will be filled with null bytes.
 * Also, if 'n' is less than the length of 'src', the resulting string will not be null-terminated.
 *
 * @param dest The destination buffer where the copied string will be stored.
 * @param src The source string to be copied.
 * @param n The number of characters to be copied from the source string to the buffer.
 * @return The destination buffer.
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
 * @param n The maximum size of the destination buffer, including the null terminator.
 * @return The destination buffer.
 */
char *strlcpy(char* dest, const char *src, size_t n);

/**
 * @brief Checks if the string consists only of lowercase letters.
 * @param c Pointer to the string to perform check on.
 * @return Returns 1 if all characters are lowercase, 0 otherwise.
 * @note The input string should be null-terminated. The check is case-sensitive.
 */
int str_is_lowercase(const char *c);

/**
 * @brief Checks if the string consists only of uppercase letters.
 * @param c Pointer to the string to perform check on.
 * @return Returns 1 if all characters are uppercase, 0 otherwise.
 * @note The input string should be null-terminated. The check is case-sensitive.
 */
int str_is_uppercase(const char *c);

/**
 * @brief Checks if the string consists only of numeric characters.
 * @param c Pointer to the string to perform check on.
 * @return Returns 1 if all characters are numeric, 0 otherwise.
 * @note The input string should be null-terminated.
 */
int str_is_numeric(const char *c);

/**
 * @brief Converts a string to an uppercase string.
 *
 * The `strupr()` function takes a string as input and converts all the
 * lowercase characters in the string to uppercase. The function modifies
 * the input string in-place and also returns a pointer to it. It doesn't
 * return a new string but rather updates the input string itself.
 *
 * Here is an example usage:
 *
 *     char my_string[] = "Hello, World!";
 *     strupr(my_string);
 *     printf("%s\n", my_string);  // Outputs: "HELLO, WORLD!"
 *
 * @param s Pointer to the string to be converted to uppercase. The string
 * should be null-terminated. This string is modified in-place.
 *
 * @return Returns the pointer to the input string `s` which has been
 * converted to uppercase.
 *
 * @note The function does not allocate any dynamic memory, so you do not
 * need to free the returned string.
 *
 * @warning The function modifies the input string. Ensure the string `s`
 * is modifiable and not a string literal.
 */
char* strupr(char* s);

/**
 * @brief Converts a string to a lowercase string.
 *
 * The `strlwr()` function takes a string as input and converts all the
 * uppercase characters in the string to lowercase. The function modifies
 * the input string in-place and also returns a pointer to it. It doesn't
 * return a new string but rather updates the input string itself.
 *
 * Here is an example usage:
 *
 *     char my_string[] = "HELLO, WORLD!";
 *     strlwr(my_string);
 *     printf("%s\n", my_string);  // Outputs: "hello, world!"
 *
 * @param s Pointer to the string to be converted to lowercase. The string
 * should be null-terminated. This string is modified in-place.
 *
 * @return Returns the pointer to the input string `s` which has been
 * converted to lowercase.
 *
 * @note The function does not allocate any dynamic memory, so you do not
 * need to free the returned string.
 *
 * @warning The function modifies the input string. Ensure the string `s`
 * is modifiable and not a string literal.
 */
char* strlwr(char* s);

/**
 * @brief Capitalizes the first letter of each word in the given string.
 *
 * This function takes a pointer to a string as input and modifies the string
 * in place to capitalize the first letter of each word in the string. Words are
 * assumed to be separated by spaces.
 *
 * @param s A pointer to the string to capitalize. This string must be null-terminated,
 *          and is modified in place by the function.
 * @return Returns a pointer to the modified string.
 */
char* strcapitalize(char* s);

/**
 * @brief Compares two C-Style strings.
 *
 * This function compares the C strings lhs and rhs.
 *
 * @param lhs A pointer to the C-Style string to be compared.
 * @param rhs A pointer to the other C-Style string to be compared.
 * @return An integer less than, equal to, or greater than zero if lhs is found, respectively, to be less than, to match, or be greater than rhs.
 */
int strcmp(const char *lhs, const char *rhs);

/**
 * @brief Compare part of two null-terminated byte strings
 *
 * @param lhs Pointer to the first character of the left-hand side string
 * @param rhs Pointer to the first character of the right-hand side string
 * @param count Maximum number of characters to compare
 *
 * The function compares up to `count` bytes of two null-terminated byte
 * strings. The comparison is done lexicographically.
 *
 * @return Returns an integer less than, equal to, or greater than zero if lhs is
 * found, respectively, to be less than, to match, or be greater than rhs.
 */
int strncmp(const char *lhs, const char *rhs, size_t count);

/**
 * @brief Locates the first occurrence of a character in a string.
 *
 * This function returns a pointer to the first occurrence of the character c in the string s.
 *
 * @param s The string to be scanned.
 * @param c The character to be searched in s.
 *
 * @return returns a pointer to the matched character or NULL if the character is not found.
 */
char *strchr(const char *s, int c);

/**
 * @brief Locates the last occurrence of a character in a string.
 *
 * The strrchr() function returns a pointer to the last occurrence of
 * the character c in the string s. The null-terminating character is
 * considered part of the string; hence, searching for '\0' will return a
 * pointer to the terminator.
 *
 * @param s This is the C string with the character to be located. It is
 * interpreted as an array of characters (byte string).
 * @param c This is the character to be located. It is passed as its int
 * promotion, but it's internally converted back to char for the comparison.
 * @return This function returns a pointer to the last occurrence of the
 * character in the string, or nullptr if the character is not found.
 */
char *strrchr(const char *s, int c);

/**
 * @brief This function searches for the first occurrence of the substring 'needle' in the string 'haystack'.
 *
 * @param haystack Pointer to the string where to search.
 * @param needle Pointer to the string to be found.
 * @return A pointer to the first occurrence of the substring 'needle' in the string 'haystack'.
 * If the substring is not found, the function returns a null pointer.
 *
 * Important note, it does not handle null pointers passed as parameters. The behavior in this case is undefined.
 */
char *strstr(const char *haystack, const char *needle);

/**
 * @brief Concatenates the source string to the destination string.
 *
 * The strcat() function appends the src string to the dest string, overwriting the '\0' character at the
 * end of dest, and then adds a terminating '\0' character. The strings may not overlap, and the dest string
 * must have enough space for the result.
 *
 * @param dest This is the pointer to the destination string, which should contain a C string,
 *        and be large enough to contain the concatenated result.
 * @param src This is the string to be appended. This should not overlap with dest.
 *
 * @return This function returns the pointer to the destination string.
 */
char *strcat(char *dest, const char *src);


/**
 * @brief This function concatenates n characters of the string pointed to by src to the end of the string pointed to by dest.
 *
 * @param dest A pointer to the destination string, which should contain a C string, and be large enough to contain the concatenated resulting string.
 * @param src A pointer to the source string. This should be a C string and not be a null pointer.
 * @param n Maximum number of characters to be appended. size_t is an unsigned integral type.
 * @return A pointer to the destination string dest.
 */
char *strncat(char *dest, const char *src, size_t n);

/**
 * @brief Calculates the length of the string pointed to by 's' until but not including
 * the terminating null character.
 *
 * @param s This is the string whose length is to be found.
 *
 * @return This function returns the length of string.
 */
long unsigned int	 strlen(const char *s);

/**
 * @brief Converts the input string to its corresponding integer value.
 *
 * This function takes a string that represents a valid integer and converts it to the corresponding integer value.
 * If the string does not represent a valid integer, or it is empty, behavior is undefined.
 *
 * @param str A pointer to a null-terminated string representing a valid integer.
 *
 * @return The integer represented by the input string.
 */
int  atoi(const char *str);

/**
 * @brief Duplicate a string.
 *
 * This function creates a duplicate of the string pointed to by `s`.
 *
 * @param s A pointer to the string to duplicate.
 * @return A pointer to the newly allocated string, or NULL if the allocation fails.
 * @note The returned pointer must be freed with `free()`.
 */
char *strdup(const char *s);
/**
 * @brief Joins multiple strings into one, using a separator.
 *
 * This function concatenates 'count' strings in 'strings' array, inserting
 * 'separator' string between each part. The result is a single string
 * which is allocated using 'malloc'. Therefore, it is the caller's responsibility
 * to free the memory.
 *
 * @param count: The number of strings in the 'strings' array.
 * @param strings: An array of pointers to strings to be joined.
 * @param separator: A string to be inserted between each string from 'strings'.
 * @return A pointer to the resulting string.
 */
char *strjoin(const int count, const char **strings, const char *separator);

#ifdef __cplusplus
}
#undef restrict
#endif

#endif