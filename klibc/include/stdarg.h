//
// Created by Emil Glenn Karpawitz on 8/26/23.
//

#ifndef BOLT_KLIBC_INCLUDE_STDARG_H_
#define BOLT_KLIBC_INCLUDE_STDARG_H_
# ifndef __va_list
#  if defined(__GNUC__) && __GNUC__ >= 3
#   define __va_list __builtin_va_list
#  else
#   define __va_list char *
#  endif
typedef __va_list va_list;
# endif

# if defined(__GNUC__) && __GNUC__ >= 3
#  define va_start(v, l)    __builtin_va_start(v,l)
#  define va_end        __builtin_va_end
#  define va_arg        __builtin_va_arg
#  define va_copy(d, s)    __builtin_va_copy(d,s)
# else
// WARNING: This might fail sometimes, but it's better than nothing.
// And with sometimes it failed until yet every time.
#  define va_start(ap, arg)	((ap) = ((va_list)&(arg)) \
		+ ((sizeof(long) > sizeof(arg)) ? sizeof(long) : sizeof(arg)))
#  define va_arg(ap, type)	*(type *)(ap), \
	((ap) += ((sizeof(long) > sizeof(type)) ? sizeof(long) : sizeof(type)))
#  define va_end(ap)
#  define va_copy(copy, arg)	((copy) = (arg))
# endif

#endif //BOLT_KLIBC_INCLUDE_STDARG_H_
