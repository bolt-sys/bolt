#pragma once

#include "ProcessorBind.h"

#define NULL          ((void *)0)

// ---------------------------------------------------------------- Prototyping

#define IN            // Indicates that this value will be used as input
#define OUT           // Indicates that it will be used as output
#define OPTIONAL      // May be NULL

// -------------------------------------------------------------------- Boolean

#define BOOLEAN       _Bool

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

// All System Level APIs should be declared with this attribute.
#define SYSAPI        __attribute__((sysv_abi))

// ------------------------------------------------------------------- Typedefs

typedef VOID*        HANDLE;
#define NULL_HANDLE ((HANDLE) NULL)

typedef unsigned int STATUS;

// --------------------------------------------------------------------- Macros

#define STRINGIFY(_X)            #_X
#define TOSTRING(_X)             STRINGIFY(_X)
#define CONCAT(_X, _Y)           _X ## _Y
#define CONCAT3(_X, _Y, _Z)      _X ## _Y ## _Z
#define CONCAT4(_X, _Y, _Z, _W)  _X ## _Y ## _Z ## _W

#define UNUSED(_X)               (void)(_X)
#define ARRAY_SIZE(_X)           (sizeof(_X) / sizeof((_X)[0]))

#define ALIGN_DOWN(_X, _Y)       ((_X) & ~((_Y) - 1))
#define ALIGN_UP(_X, _Y)         ALIGN_DOWN((_X) + (_Y) - 1, (_Y))
#define IS_ALIGNED(_X, _Y)       (((_X) & ((_Y) - 1)) == 0)

#define MIN(_X, _Y)              ((_X) < (_Y) ? (_X) : (_Y))
#define MAX(_X, _Y)              ((_X) > (_Y) ? (_X) : (_Y))
#define CLAMP(_X, _MIN, _MAX)    MIN(MAX(_X, _MIN), _MAX)

#define BIT(_X)                  (1ULL << (_X))
#define BIT_IS_SET(_X, _Y)       (((_X) & BIT(_Y)) != 0)
#define BIT_IS_CLEAR(_X, _Y)     (((_X) & BIT(_Y)) == 0)
#define BIT_SET(_X, _Y)          ((_X) |= BIT(_Y))
#define BIT_CLEAR(_X, _Y)        ((_X) &= ~BIT(_Y))
#define BIT_FLIP(_X, _Y)         ((_X) ^= BIT(_Y))

#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg " (" __FILE__ ":" TOSTRING(__LINE__) ")")

#define CONTAINER_OF(_ptr, _type, _member) \
    ((_type *)((char *)(_ptr) - offsetof(_type, _member)))

// --------------------------------------------------------------- Type Testing

STATIC_ASSERT (sizeof (INT8)   == 1, "Invalid size for INT8");
STATIC_ASSERT (sizeof (INT16)  == 2, "Invalid size for INT16");
STATIC_ASSERT (sizeof (INT32)  == 4, "Invalid size for INT32");
STATIC_ASSERT (sizeof (INT64)  == 8, "Invalid size for INT64");

STATIC_ASSERT (sizeof (UINT8)  == 1, "Invalid size for UINT8");
STATIC_ASSERT (sizeof (UINT16) == 2, "Invalid size for UINT16");
STATIC_ASSERT (sizeof (UINT32) == 4, "Invalid size for UINT32");
STATIC_ASSERT (sizeof (UINT64) == 8, "Invalid size for UINT64");

STATIC_ASSERT (sizeof (UINTN)  == sizeof (void*), "Invalid size for UINTN");
STATIC_ASSERT (sizeof (INTN)   == sizeof (void*), "Invalid size for INTN");

// -------------------------------------------------------------------- VA List

typedef __builtin_va_list VA_LIST;

#define VA_START(_ap, _argN) __builtin_va_start(_ap, _argN)
#define VA_END(_ap)          __builtin_va_end(_ap)
#define VA_ARG(_ap, _type)   __builtin_va_arg(_ap, _type)
#define VA_COPY(_dst, _src)  __builtin_va_copy(_dst, _src)

// ----------------------------------------------------------------- Re-exports

#include "Error.h"   // Status codes
#include "Memory.h"  // Common memory operations
#include "String.h"  // Common string operations
