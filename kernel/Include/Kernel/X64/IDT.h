#pragma once

#ifndef __KERNEL_H__
    #error "This file should be included from Kernel.h"
#endif

typedef struct {
    UINT16 BaseLow;
    UINT16 Selector;
    UINT8  IST;
    UINT8  TypeAttribute;
    UINT16 BaseHigh;
} PACKED IDT_ENTRY32;

typedef struct {
    IDT_ENTRY32 Low;
    UINT32      BaseHighExtended;
    UINT32      Reserved;
} PACKED IDT_ENTRY64;

typedef struct {
    UINT16 Limit;
    UINTN  Base;
} PACKED IDTR;

typedef struct {
    UINTN ES, DS;
    UINTN R15, R14, R13, R12, R11, R10, R9, R8;
    UINTN SI, DI, BP, DX, CX, BX, AX;
    UINTN VectorNum, ErrorCode;
    UINTN IP, CS, FLAGS, SP, SS;
} PACKED INTERRUPT_FRAME;

#define IDT_ATTR_PRESENT        BIT(7)
#define IDT_ATTR_RING_0         0
#define IDT_ATTR_RING_1         BIT(5)
#define IDT_ATTR_RING_2         BIT(6)
#define IDT_ATTR_RING_3         (BIT(5) | BIT(6))

#define IDT_TYPE_INTERRUPT_GATE (IDT_ATTR_PRESENT | IDT_ATTR_RING_0 | 0b1110)
#define IDT_TYPE_TRAP_GATE      (IDT_ATTR_PRESENT | IDT_ATTR_RING_0 | 0b1111)

#define IDT_ENTRY_COUNT         256

EXTERN
SYSAPI
VOID
DisableInterrupts (
    VOID
    );

EXTERN
SYSAPI
VOID
EnableInterrupts (
    VOID
    );

EXTERN
SYSAPI
VOID
PopulateISRHandlers (
    VOID
    );

/**
 * @brief Initializes the IDT.
 **/
VOID
SYSAPI
IDTInit (
    VOID
    );

/**
 * @brief Loads the IDT.
 *
 * @param[in] IDTR The IDT register.
 **/
VOID
SYSAPI
IDTLoad (
    IN IDTR* IDTR
    );
