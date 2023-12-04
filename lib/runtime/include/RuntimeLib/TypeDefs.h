/**
 * @file TypeDefs.h
 * @brief This file contains type definitions, macros, and attributes used throughout the RuntimeLib library.
 *
 * This file should be included from RuntimeLib.h. It defines several macros for indicating input/output parameters,
 * boolean values, consistency, attributes, and typedefs for various integer types and character encodings.
 * It also defines several macros for string manipulation, bit manipulation, and container manipulation.
 *
 * All public system level APIs should be declared with the SYSAPI attribute, while all private/internal/local APIs
 * should be declared with the LOCALAPI attribute.
 *
 * This file is intended to be used with GCC and Clang compilers.
 */
#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END   }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

EXTERN_C_START

#ifdef NULL
    #undef NULL
#endif

#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif

// ---------------------------------------------------------------- Prototyping

#define IN       // Indicates that this value will be used as input
#define OUT      // Indicates that it will be used as output
#define OPTIONAL // May be NULL

// -------------------------------------------------------------------- Boolean

#ifdef __cplusplus
#define BOOLEAN       bool
#else
#define BOOLEAN       _Bool
#endif

#define TRUE          ((BOOLEAN)(1 == 1))
#define FALSE         ((BOOLEAN)(1 == 0))

// ---------------------------------------------------------------- Consistency

#define STATIC        static
#define INLINE        inline
#define VOID          void
#define CONST         const
#define EXTERN        extern
#define VOLATILE      volatile

// ----------------------------------------------------------------- Attributes

#define NORETURN      __attribute__((noreturn))
#define PACKED        __attribute__((packed))
#define WEAK          __attribute__((weak))
#define ALIGNED(_X) __attribute__((aligned(_X)))
#define NOINLINE      __attribute__((noinline))
#define ALWAYS_INLINE __attribute__((always_inline))
#define PURE          __attribute__((pure))

// All *PUBLIC* System Level APIs should be declared with this attribute.
#define SYSAPI        __attribute__((sysv_abi)) __attribute__((visibility("default")))

// All *PRIVATE/INTERNAL/LOCAL* APIs should be declared with this attribute.
#define LOCALAPI      __attribute__((sysv_abi)) __attribute__((visibility("hidden")))

// ------------------------------------------------------------------- Typedefs

typedef VOID*        HANDLE;
#define NULL_HANDLE ((HANDLE) NULL)

typedef unsigned int STATUS;

// --------------------------------------------------------------------- Macros

#define STRINGIFY(_X)                               #_X

#define CONCAT1(_X)                                 _X
#define CONCAT2(_X, _Y)                             _X ## _Y
#define CONCAT3(_X, _Y, _Z)                         _X ## _Y ## _Z
#define CONCAT4(_X, _Y, _Z, _W)                     _X ## _Y ## _Z ## _W
#define CONCAT5(_X, _Y, _Z, _W, _V)                 _X ## _Y ## _Z ## _W ## _V
#define CONCAT6(_X, _Y, _Z, _W, _V, _U)             _X ## _Y ## _Z ## _W ## _V ## _U
#define CONCAT7(_X, _Y, _Z, _W, _V, _U, _T)         _X ## _Y ## _Z ## _W ## _V ## _U ## _T
#define CONCAT8(_X, _Y, _Z, _W, _V, _U, _T, _S)     _X ## _Y ## _Z ## _W ## _V ## _U ## _T ## _S
#define CONCAT9(_X, _Y, _Z, _W, _V, _U, _T, _S, _R) _X ## _Y ## _Z ## _W ## _V ## _U ## _T ## _S ## _R

#define UNUSED(_X)                                  (void)(_X)
#define ARRAY_SIZE(_X)                              (sizeof(_X) / sizeof((_X)[0]))

#define ALIGN_DOWN(_X, _Y)                          ((_X) & ~((_Y) - 1))
#define ALIGN_UP(_X, _Y)                            ALIGN_DOWN((_X) + (_Y) - 1, (_Y))
#define IS_ALIGNED(_X, _Y)                          (((_X) & ((_Y) - 1)) == 0)

#define MIN(_X, _Y)                                 ((_X) < (_Y) ? (_X) : (_Y))
#define MAX(_X, _Y)                                 ((_X) > (_Y) ? (_X) : (_Y))
#define CLAMP(_X, _MIN, _MAX)                       MIN(MAX(_X, _MIN), _MAX)

#define BIT(_X)                                     (1ULL << (_X))
#define BIT_IS_SET(_X, _Y)                          (((_X) & BIT(_Y)) != 0)
#define BIT_IS_CLEAR(_X, _Y)                        (((_X) & BIT(_Y)) == 0)
#define BIT_SET(_X, _Y)                             ((_X) |= BIT(_Y))
#define BIT_CLEAR(_X, _Y)                           ((_X) &= ~BIT(_Y))
#define BIT_FLIP(_X, _Y)                            ((_X) ^= BIT(_Y))

#ifndef __cplusplus
#define STATIC_ASSERT(_COND, _MSG)                  _Static_assert(_COND, _MSG " (" __FILE__ ":" STRINGIFY(__LINE__) ")")
#else
#define STATIC_ASSERT(_COND, _MSG)                  static_assert(_COND, _MSG " (" __FILE__ ":" STRINGIFY(__LINE__) ")")
#endif

#define CONTAINER_OF(_PTR, _TYPE, _MEMBER)          ((_TYPE *)((char *)(_PTR) - offsetof(_TYPE, _MEMBER)))

// ------------------------------------------------------------------- Typedefs

// This only really works on GCC and Clang, thus those are the only compilers
// that we support.

typedef __INT8_TYPE__  INT8;
typedef __INT16_TYPE__ INT16;
typedef __INT32_TYPE__ INT32;
typedef __INT64_TYPE__ INT64;

STATIC_ASSERT (sizeof (INT8) == 1, "Invalid size for INT8");
STATIC_ASSERT (sizeof (INT16) == 2, "Invalid size for INT16");
STATIC_ASSERT (sizeof (INT32) == 4, "Invalid size for INT32");
STATIC_ASSERT (sizeof (INT64) == 8, "Invalid size for INT64");

typedef __UINT8_TYPE__  UINT8;
typedef __UINT16_TYPE__ UINT16;
typedef __UINT32_TYPE__ UINT32;
typedef __UINT64_TYPE__ UINT64;

STATIC_ASSERT (sizeof (UINT8) == 1, "Invalid size for UINT8");
STATIC_ASSERT (sizeof (UINT16) == 2, "Invalid size for UINT16");
STATIC_ASSERT (sizeof (UINT32) == 4, "Invalid size for UINT32");
STATIC_ASSERT (sizeof (UINT64) == 8, "Invalid size for UINT64");

typedef char           CHAR8;  // UTF-8, ASCII,
typedef unsigned short CHAR16; // UTF-16, UCS-2
typedef unsigned int   CHAR32; // UTF-32, UCS-4

STATIC_ASSERT (sizeof (CHAR8) == 1, "Invalid size for CHAR8");
STATIC_ASSERT (sizeof (CHAR16) == 2, "Invalid size for CHAR16");
STATIC_ASSERT (sizeof (CHAR32) == 4, "Invalid size for CHAR32");

// UINTN really depends on the platform that we're compiling for. It should be
// the same size as a pointer. This is because it's used to store pointers or
// pointer arithmetic like sizes and offsets.

typedef __PTRDIFF_TYPE__ INTN;
typedef __SIZE_TYPE__    UINTN;

typedef __INTMAX_TYPE__  INTMAX;
typedef __UINTMAX_TYPE__ UINTMAX;

typedef VOID*            HANDLE;

typedef UINT32           STATUS;

// --------------------------------------------------------------------- Limits

//
// Maximum Limits
//

#undef INT8_MAX
#undef INT16_MAX
#undef INT32_MAX
#undef INT64_MAX
#undef UINT8_MAX
#undef UINT16_MAX
#undef UINT32_MAX
#undef UINT64_MAX
#undef INTN_MAX
#undef UINTN_MAX
#undef INTMAX_MAX
#undef UINTMAX_MAX
#undef CHAR8_MAX
#undef CHAR16_MAX
#undef CHAR32_MAX

#undef INT8_MIN
#undef INT16_MIN
#undef INT32_MIN
#undef INT64_MIN
#undef UINT8_MIN
#undef UINT16_MIN
#undef UINT32_MIN
#undef UINT64_MIN
#undef INTN_MIN
#undef UINTN_MIN
#undef INTMAX_MIN
#undef UINTMAX_MIN
#undef CHAR8_MIN
#undef CHAR16_MIN
#undef CHAR32_MIN

#define INT8_MAX    __INT8_MAX__
#define INT16_MAX   __INT16_MAX__
#define INT32_MAX   __INT32_MAX__
#define INT64_MAX   __INT64_MAX__

#define UINT8_MAX   __UINT8_MAX__
#define UINT16_MAX  __UINT16_MAX__
#define UINT32_MAX  __UINT32_MAX__
#define UINT64_MAX  __UINT64_MAX__

#define INTN_MAX    __PTRDIFF_MAX__
#define UINTN_MAX   __SIZE_MAX__

#define INTMAX_MAX  __INTMAX_MAX__
#define UINTMAX_MAX __UINTMAX_MAX__

#define CHAR8_MAX   INT8_MAX
#define CHAR16_MAX  UINT16_MAX
#define CHAR32_MAX  UINT32_MAX

//
// Minimum Limits
//

// Last bit is the sign bit, thus we can just flip all the bits to get
// the minimum value.
// E.G 0x7F (0111 1111) becomes 0x80 (1000 0000)
#define INT8_MIN    (~INT8_MAX)
#define INT16_MIN   (~INT16_MAX)
#define INT32_MIN   (~INT32_MAX)
#define INT64_MIN   (~INT64_MAX)

#define UINT8_MIN   (~UINT8_MAX)
#define UINT16_MIN  (~UINT16_MAX)
#define UINT32_MIN  (~UINT32_MAX)
#define UINT64_MIN  (~UINT64_MAX)

#define INTN_MIN    (~INTN_MAX)
#define UINTN_MIN   (~UINTN_MAX)

#define INTMAX_MIN  (~INTMAX_MAX)
#define UINTMAX_MIN (~UINTMAX_MAX)

#define CHAR8_MIN   INT8_MIN
#define CHAR16_MIN  INT16_MIN
#define CHAR32_MIN  INT32_MIN

// -------------------------------------------------------------------- VA List

typedef __builtin_va_list VA_LIST;

#define VA_START(_ap, _argN) __builtin_va_start(_ap, _argN)
#define VA_END(_ap)          __builtin_va_end(_ap)
#define VA_ARG(_ap, _type)   __builtin_va_arg(_ap, _type)
#define VA_COPY(_dst, _src)  __builtin_va_copy(_dst, _src)

EXTERN_C_END
