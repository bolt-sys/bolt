#pragma once

typedef char               INT8;
typedef short              INT16;
typedef int                INT32;
typedef long long          INT64;

typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef unsigned long long UINT64;

typedef UINT64             UINTN;
typedef INT64              INTN;

typedef INT8               CHAR8;
typedef UINT16             CHAR16;
typedef UINT32             CHAR32;
typedef UINT64             CHAR64;

#define INT8_MAX    0x7f
#define INT16_MAX   0x7fff
#define INT32_MAX   0x7fffffff
#define INT64_MAX   0x7fffffffffffffffLL

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffffU
#define UINT64_MAX  0xffffffffffffffffULL

#define INTN_MAX    INT64_MAX
#define UINTN_MAX   UINT64_MAX

#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define CHAR8_MAX   INT8_MAX
#define CHAR16_MAX  UINT16_MAX
#define CHAR32_MAX  UINT32_MAX
#define CHAR64_MAX  UINT64_MAX
