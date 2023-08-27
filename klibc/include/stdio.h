//
// Created by Emil Glenn Karpawitz on 8/26/23.
//

#ifndef BOLT_KLIBC_INCLUDE_STDIO_H_
#define BOLT_KLIBC_INCLUDE_STDIO_H_

#include "stdarg.h"
/**
 * @brief The printf function sends formatted output to stdout
 *
 * @param format This is a string that contains the text to be written to the stdout.
 *               It can optionally contain embedded format specifiers that are replaced by
 *               the values specified in the additional arguments and formatted as requested.
 *               Format specifiers include: special character escapes (like \n for newline),
 *               %s for string, %d for integer, etc.
 *
 * @param ... This means that the function can take any number of additional arguments,
 *            which represent values to be substituted into the format specifiers in the format string.
 *
 * @return void This function does not return a value.
 */
void printf(const char *format, ...);


#endif //BOLT_KLIBC_INCLUDE_STDIO_H_
