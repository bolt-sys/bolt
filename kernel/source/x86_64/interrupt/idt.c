//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#include "idt.h"

IDT64 idt64[256];

IDT64_PTR idt64_ptr = (IDT64_PTR){
		.limit = sizeof(idt64) - 1,
		.base = (uint64_t) &idt64
};

void load_idt64(void)
{
	asm volatile("lidt %0" : : "m"(idt64_ptr));
}

// TODO add diffrent types of gates? not sure if needed
void idt_set_gate(int n, uint64_t handler)
{
	idt64[n] = (IDT64){
			.offset_1 = handler & 0xFFFF,
			.selector = 0x08,
			.ist = 0,
			.type_attributes = 0x8E,
			.offset_2 = (handler >> 16) & 0xFFFF,
			.offset_3 = (handler >> 32) & 0xFFFFFFFF,
			._ = 0
	};
}
