//
// Created by Emil Karpawitz on 8/24/23.
//

#include "../include/GDT.h"


GDT gdt[6];
GDT_PTR gdt_ptr;

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void gdt_flush()
{
	asm volatile("lgdt %0\n\t"
				 "mov $0x10, %%ax\n\t"
				 "mov %%ax, %%ds\n\t"
				 "mov %%ax, %%es\n\t"
				 "mov %%ax, %%fs\n\t"
				 "mov %%ax, %%gs\n\t"
				 "mov %%ax, %%ss\n\t"
				 "leaq 0x8(%%rsp), %%rsp\n\t"
				 "jmp $.1(%%rsp)\n\t"
				 ".1:\n\t"
				 : : "m" (gdt_ptr)  );
}

void gdt_install()
{
	gdt_ptr.limit = (sizeof(GDT) * 3) - 1;
	gdt_ptr.base = &gdt;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment


	gdt_flush();
}