//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#include "isr.h"

static const char* interrupt[] = {
	"Divide by zero",
	"Debug",
	"Non maskable interrupt",
	"Breakpoint",
	"Overflow",
	"Bound range exceeded",
	"Invalid opcode",
	"Device not available",
	"Double fault HOW TF DID YOU GET HERE",
	"Coprocessor segment overrun",
	"Invalid TSS",
	"Segment not present",
	"Stack segment fault",
	"General protection fault",
	"Page fault",
	"Reserved",
	"x87 floating point exception",
	"Alignment check",
	"Machine check",
	"SIMD floating point exception",
	"Virtualization exception",
	"Control protection exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

extern void puts(char* s);

static void putc(char c)
{
	puts(&c);
}

static void putnbr(uint64_t n)
{
	if (n < 0) {
		puts("-");
		n = -n;
	}
	if (n > 9) {
		putnbr(n / 10);
	}
	char c = n % 10 + '0';
	putc(c);
}

void isr_handler(registers* r)
{
	puts("Interrupt: ");
	if (r->int_no < 32) {
		puts(interrupt[r->int_no]);
	}
	else {
		putnbr(r->int_no);
	}
	puts(" received\n");
}