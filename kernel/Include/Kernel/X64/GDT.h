#pragma once

#ifndef __KERNEL_H__
    #error "This file should be included from Kernel.h"
#endif

#include <RuntimeLib.h>

typedef struct {
    UINT16 LimitLow;
    UINT16 BaseLow;
    UINT8  BaseMiddle;
    UINT8  Access;
    UINT8  Granularity;
    UINT8  BaseHigh;
} PACKED GDT_ENTRY32;

typedef struct {
    GDT_ENTRY32 Low;
    UINT32      BaseHighExtended;
    UINT32      Reserved;
} PACKED GDT_ENTRY64;

typedef struct {
    UINT16 Limit;
    UINTN  Base;
} PACKED GDTR;

typedef struct {
    GDT_ENTRY64 Null;

    GDT_ENTRY64 KernelCode;
    GDT_ENTRY64 KernelData;

    GDT_ENTRY64 UserCode;
    GDT_ENTRY64 UserData;

    GDT_ENTRY64 TSS;
} PACKED GDT;

#define GDT_ACCESS_PRESENT    BIT(7)
#define GDT_ACCESS_PRIVILEGE0 0
#define GDT_ACCESS_PRIVILEGE1 BIT(5)
#define GDT_ACCESS_PRIVILEGE2 BIT(6)
#define GDT_ACCESS_PRIVILEGE3 (GDT_ACCESS_PRIVILEGE1 | GDT_ACCESS_PRIVILEGE2)
#define GDT_ACCESS_SYSTEM     BIT(4)
#define GDT_ACCESS_EXECUTABLE BIT(3)
#define GDT_ACCESS_DIRECTION  BIT(2)
#define GDT_ACCESS_READWRITE  BIT(1)
#define GDT_ACCESS_ACCESSED   BIT(0)

#define GDT_GRANULARITY_1B    0
#define GDT_GRANULARITY_4KB   BIT(7)
#define GDT_GRANULARITY_16BIT 0
#define GDT_GRANULARITY_32BIT BIT(6)
#define GDT_GRANULARITY_64BIT BIT(5)
#define GDT_GRANULARITY_AVL   BIT(4)

#define GDT_NULL32            GDT_SEGMENT32(0, 0, 0, 0)
#define GDT_NULL64            GDT_SEGMENT64(0, 0, 0, 0)

#define GDT_SEGMENT32(_BASE, _LIMIT, _ACCESS, _GRANULARITY)                 \
    {                                                                       \
        .LimitLow    = (UINT16)((_LIMIT) & 0xFFFF),                         \
        .BaseLow     = (UINT16)((_BASE) & 0xFFFF),                          \
        .BaseMiddle  = (UINT8)(((_BASE) >> 16) & 0xFF),                     \
        .Access      = (UINT8)((_ACCESS) & 0xFF),                           \
        .Granularity = (UINT8)((_GRANULARITY) | (((_LIMIT) >> 16) & 0x0F)), \
        .BaseHigh    = (UINT8)(((_BASE) >> 24) & 0xFF)                      \
    }

#define GDT_SEGMENT64(_BASE, _LIMIT, _ACCESS, _GRANULARITY)                      \
    {                                                                            \
        .Low              = GDT_SEGMENT32(_BASE, _LIMIT, _ACCESS, _GRANULARITY), \
        .BaseHighExtended = (UINT32)((((UINT64)_BASE) >> 32) & 0xFFFFFFFF),      \
        .Reserved         = 0                                                    \
    }

#define GDT_NULL      0x00
#define GDT_KERNEL_CS 0x10
#define GDT_KERNEL_DS 0x20
#define GDT_USER_CS   0x30
#define GDT_USER_DS   0x40

/**
 * @brief Initialize the GDT
 **/
VOID
SYSAPI
GDTInit (
    VOID
    );

/**
 * @brief Loads a GDTR
 *
 * @param gdtr A pointer and length to the effective `GDT`
 */
VOID
SYSAPI
GDTLoad (
    GDTR* gdtr
    );

/**
 * @brief Reloads the segments
 *
 * @param CS Code Segment
 * @param DS Data Segment
 **/
VOID
SYSAPI
GDTReloadSegments (
    UINT16 CS,
    UINT16 DS
    );
