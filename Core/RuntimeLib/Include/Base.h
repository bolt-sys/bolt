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

// -------------------------------------------------------------------- Defines

#define  SIZE_1KB    0x00000400
#define  SIZE_2KB    0x00000800
#define  SIZE_4KB    0x00001000
#define  SIZE_8KB    0x00002000
#define  SIZE_16KB   0x00004000
#define  SIZE_32KB   0x00008000
#define  SIZE_64KB   0x00010000
#define  SIZE_128KB  0x00020000
#define  SIZE_256KB  0x00040000
#define  SIZE_512KB  0x00080000
#define  SIZE_1MB    0x00100000
#define  SIZE_2MB    0x00200000
#define  SIZE_4MB    0x00400000
#define  SIZE_8MB    0x00800000
#define  SIZE_16MB   0x01000000
#define  SIZE_32MB   0x02000000
#define  SIZE_64MB   0x04000000
#define  SIZE_128MB  0x08000000
#define  SIZE_256MB  0x10000000
#define  SIZE_512MB  0x20000000
#define  SIZE_1GB    0x40000000
#define  SIZE_2GB    0x80000000
#define  SIZE_4GB    0x0000000100000000ULL
#define  SIZE_8GB    0x0000000200000000ULL
#define  SIZE_16GB   0x0000000400000000ULL
#define  SIZE_32GB   0x0000000800000000ULL
#define  SIZE_64GB   0x0000001000000000ULL
#define  SIZE_128GB  0x0000002000000000ULL
#define  SIZE_256GB  0x0000004000000000ULL
#define  SIZE_512GB  0x0000008000000000ULL
#define  SIZE_1TB    0x0000010000000000ULL
#define  SIZE_2TB    0x0000020000000000ULL
#define  SIZE_4TB    0x0000040000000000ULL
#define  SIZE_8TB    0x0000080000000000ULL
#define  SIZE_16TB   0x0000100000000000ULL
#define  SIZE_32TB   0x0000200000000000ULL
#define  SIZE_64TB   0x0000400000000000ULL
#define  SIZE_128TB  0x0000800000000000ULL
#define  SIZE_256TB  0x0001000000000000ULL
#define  SIZE_512TB  0x0002000000000000ULL
#define  SIZE_1PB    0x0004000000000000ULL
#define  SIZE_2PB    0x0008000000000000ULL
#define  SIZE_4PB    0x0010000000000000ULL
#define  SIZE_8PB    0x0020000000000000ULL
#define  SIZE_16PB   0x0040000000000000ULL
#define  SIZE_32PB   0x0080000000000000ULL
#define  SIZE_64PB   0x0100000000000000ULL
#define  SIZE_128PB  0x0200000000000000ULL
#define  SIZE_256PB  0x0400000000000000ULL
#define  SIZE_512PB  0x0800000000000000ULL
#define  SIZE_1EB    0x1000000000000000ULL
#define  SIZE_2EB    0x2000000000000000ULL
#define  SIZE_4EB    0x4000000000000000ULL
#define  SIZE_8EB    0x8000000000000000ULL

// ----------------------------------------------------------------------- GUID

typedef struct GUID {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];
} GUID;

#define GUID_NULL { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }

#define GUID_EQ(_X, _Y) \
    ((_X).Data1 == (_Y).Data1 && \
     (_X).Data2 == (_Y).Data2 && \
     (_X).Data3 == (_Y).Data3 && \
     (_X).Data4[0] == (_Y).Data4[0] && \
     (_X).Data4[1] == (_Y).Data4[1] && \
     (_X).Data4[2] == (_Y).Data4[2] && \
     (_X).Data4[3] == (_Y).Data4[3] && \
     (_X).Data4[4] == (_Y).Data4[4] && \
     (_X).Data4[5] == (_Y).Data4[5] && \
     (_X).Data4[6] == (_Y).Data4[6] && \
     (_X).Data4[7] == (_Y).Data4[7])

#define GUID_NEQ(_X, _Y) (!GUID_EQ(_X, _Y))
