//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#ifndef BOLT_ISR_H
#define BOLT_ISR_H

#include "stdint.h"
#include "idt.h"

typedef struct
{
	uint64_t es, ds;
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
	uint64_t error_no, int_no;
	uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) registers;

void isr_handler(registers* r);

#endif //BOLT_ISR_H
