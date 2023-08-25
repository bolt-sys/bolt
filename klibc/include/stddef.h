//
// Created by Robin Alexander Plate on 8/25/23.
//

#ifndef BOLT_KLIBC_STDDEF_H_
#define BOLT_KLIBC_STDDEF_H_

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __UINTPTR_TYPE__ uintptr_t;
typedef __INTPTR_TYPE__ intptr_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif // BOLT_KLIBC_STDDEF_H_
