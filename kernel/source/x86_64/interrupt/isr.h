//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#ifndef BOLT_ISR_H
#define BOLT_ISR_H

#include "stdint.h"
#include "idt.h"

typedef struct
{
	uint64_t ds;
	uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t error_code, int_no;
	uint64_t rip, cs, rflags, userrsp, ss;
} __attribute__((packed)) registers;

void isr_handler(registers* r);

#endif //BOLT_ISR_H
