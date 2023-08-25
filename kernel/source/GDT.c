//
// Created by Emil Karpawitz on 8/24/23.
//

#include "GDT.h"
#include "MemoryUtils.h"

GDT gdt[6];
GDT_PTR gdt_ptr;

// TODO: Figure out what is wrong with this function
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

void gdt_install()
{
    gdt_ptr.limit = (sizeof(GDT) * 5) - 1;
    gdt_ptr.base = (uint64_t) &gdt;
	memset(&gdt, 0, sizeof(GDT) * 5);

	/*
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	tss_install(5, 0x10, 0x0);
*/
	gdt[0] = (GDT) {0, 0, 0, 0x00, 0x00, 0};
	gdt[1] = (GDT) {0, 0, 0, 0x9a, 0xa0, 0};
	gdt[2] = (GDT) {0, 0, 0, 0x92, 0xa0, 0};
	gdt[3] = (GDT) {0, 0, 0, 0x9a, 0xa0, 0};
	gdt[4] = (GDT) {0, 0, 0, 0x92, 0xa0, 0};

	gdt_flush(&gdt_ptr);
}
