//
// Created by Emil Karpawitz on 8/24/23.
//

#include "GDT.h"
#include "TSS.h"
#include "MemoryUtils.h"

GDT gdt[3];
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

void gdt_install()
{
    gdt_ptr.limit = (sizeof(GDT) * 6) - 1;
    gdt_ptr.base = &gdt;
	memset(&gdt, 0, sizeof(GDT) * 6);

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	tss_install(5, 0x10, 0x0);

	gdt_flush();
	tss_flush();
}
