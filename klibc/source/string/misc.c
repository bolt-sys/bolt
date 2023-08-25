//
// Created by Emil Glenn Karpawitz on 8/26/23.
//
#include <string.h>

int	strlen(const char *s)
{
	int i;
	for (i = 0; s[i]; ++i);
	return (i);
}

int str_is_lowercase(const char *str)
{
	for (int i = 0; i < str[i]; ++i)
	{
		if (str[i] < 'a' || str[i] > 'z')
			return (0);
	}
	return (1);
}

int str_is_uppercase(const char *str)
{
	for (int i = 0; i < str[i]; ++i)
	{
		if (str[i] < 'A' || str[i] > 'Z')
			return (0);
	}
	return (1);
}

int str_is_numeric(const char *str)
{
	for (int i = 0; i < str[i]; ++i)
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
	}
	return (1);
}

char *strupr(char *s)
{
	for (int i = 0; s[i]; ++i)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] -= 32;
	}
	return (s);
}

char *strlwr(char *s)
{
	for (int i = 0; s[i]; ++i)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 32;
	}
	return (s);
}

char *strcapitalize(char *s)
{
	int space = 1;
	for (int i = 0; s[i]; ++i)
	{
		if (space && str_is_lowercase(&s[i]))
			s[i] += 32;
		space = (s[i] == ' ' || s[i] == '\t' || s[i] == '\n');
	}
	return (s);
}

int strcmp(const char *lhs, const char *rhs)
{
	int i;
	for (i = 0; lhs[i] || rhs[i]; ++i)
	{
		if (lhs[i] != rhs[i])
			break;
	}
	return (lhs[i] - rhs[i]);
}

int strncmp(const char *lhs, const char *rhs, size_t n)
{
	int i;
	for (i = 0; (lhs[i] || rhs[i]) && i < n; ++i)
	{
		if (lhs[i] != rhs[i])
			break;
	}
	return (lhs[i] - rhs[i]);
}

char *strchr(const char *s, int c)
{
	for (int i = 0; s[i]; ++i)
	{
		if (s[i] == c)
			return ((char*)s + i);
	}
	return (NULL);
}

char *strrchr(const char *s, int c)
{
	for (int i = strlen(s) - 1; i >= 0; --i)
	{
		if (s[i] == c)
			return ((char*)s + i);
	}
	return (NULL);
}

char *strstr(const char *haystack, const char *needle)
{
	for (int i = 0; haystack[i]; ++i)
	{
		if (strncmp(haystack + i, needle, strlen(needle)) == 0)
			return ((char*)&haystack[i]);
	}
	return (NULL);
}

int atoi(const char* str)
{
	int res = 0;
	int sign = 1;
	int i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';

	return (sign * res);
}