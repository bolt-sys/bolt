#ifndef __STDDEF_H
#define __STDDEF_H

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __UINTPTR_TYPE__ uintptr_t;
typedef __INTPTR_TYPE__ intptr_t;

#ifndef NULL
	#define NULL ((void *)0)
#endif

#endif