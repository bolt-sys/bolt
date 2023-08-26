//
// Created by Emil Glenn Karpawitz on 8/26/23.
//
#include <string.h>
#include "stdlib.h"

char *strcpy(char *dest, const char *src)
{
	return (char *)memcpy(dest, src, strlen(src) + 1);
}

char *strncpy(char *dest, const char *src, size_t count)
{
	size_t len = strlen(src);
	if (len > count)
		len = count;
	memcpy(dest, src, len);
	memset(dest + len, 0, count - len);
	return dest;
}

char *strlcpy(char *dest, const char *src, size_t count)
{
	size_t len = strlen(src);
	if (len > count - 1)
		len = count - 1;
	memcpy(dest, src, len);
	dest[len] = 0;
	return dest;
}

char *strcat(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	int i = 0;
	return dest;
}

char *strncat(char *dest, const char *src, size_t count)
{
	strncpy(dest + strlen(dest), src, count);
	return dest;
}

char *strdup(const char *src)
{
	size_t len = strlen(src);
	char *dest = malloc(len + 1);

	memcpy(dest, src, len + 1);
	return dest;
}