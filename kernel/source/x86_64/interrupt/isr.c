//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#include "isr.h"

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
	putnbr(r->int_no);
	puts("\n");
}