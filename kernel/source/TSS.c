#include "TSS.h"
#include "GDT.h"
#include "MemoryUtils.h"

tss tss_entry;

void tss_set_stack(unsigned int kernel_stack)
{
	tss_entry.esp0 = kernel_stack;
}

void tss_install(signed int num, unsigned short ss0, unsigned short esp0)
{
	unsigned long base = (unsigned long)&tss_entry;
	unsigned long limit = base + sizeof(tss_entry);

	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset((void*)&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}