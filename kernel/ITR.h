//
// Created by Emil Karpawitz on 8/24/23.
//

#ifndef BOLT_ITR_H
#define BOLT_ITR_H
#include "limine.h"

typedef struct InterruptDescriptorTable32
{
	uint16_t 	offset_1;        // offset bits 0..15
	uint16_t 	selector;        // a code segment selector in GDT or LDT
	uint8_t		_;               // unused, set to 0
	uint8_t		type_attributes; // gate type, dpl, and p fields
	uint16_t	offset_2;        // offset bits 16..31
} __attribute__((packed)) IDT32;

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

#endif //BOLT_ITR_H
