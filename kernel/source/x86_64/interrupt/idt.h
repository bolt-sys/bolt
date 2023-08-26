//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#ifndef BOLT_IDT_H
#define BOLT_IDT_H

#include "stdint.h"

typedef struct InterruptDescriptorTable64
{
	uint16_t 	offset_1;        // offset bits 0..15
	uint16_t 	selector;        // a code segment selector in GDT or LDT
	uint8_t  	ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	uint8_t  	type_attributes; // type and attributes
	uint16_t 	offset_2;        // offset bits 16..31
	uint32_t 	offset_3;        // offset bits 32..63
	uint32_t 	_;               // reserved
} __attribute__((packed)) IDT64;


typedef struct InterruptDescriptorTable64Pointer
{
	uint16_t 	limit;           // size of IDT
	uint64_t 	base;            // virtual address of IDT
} __attribute__((packed)) IDT64_PTR;

typedef enum {
	//						    P(1) DPL(2) 0(1) GATE_TYPE(4)
	INTERRUPT_GATE 			= 0b10001110,
	TRAP_GATE 				= 0b10001111,
} IDT_TYPE;


extern IDT64 idt64[256];

extern IDT64_PTR idt64_ptr;

void load_idt64();

void idt_set_entry(int n, uint64_t handler);

void stop_interrupts();

void start_interrupts();

#endif //BOLT_IDT_H
