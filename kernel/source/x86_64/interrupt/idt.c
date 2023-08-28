//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#include "idt.h"

extern void idt_init(void);

IDT64 idt64[256];

uint8_t types[32] = {
	TRAP_GATE, // Divide by zero
	TRAP_GATE, // Debug
	INTERRUPT_GATE, // Non maskable interrupt
	TRAP_GATE, // Breakpoint
	TRAP_GATE, // Overflow
	TRAP_GATE, // Bound range exceeded
	TRAP_GATE, // Invalid opcode
	TRAP_GATE, // Device not available
	TRAP_GATE, // Double fault
	TRAP_GATE, // Coprocessor segment overrun
	TRAP_GATE, // Invalid TSS
	TRAP_GATE, // Segment not present
	TRAP_GATE, // Stack segment fault
	TRAP_GATE, // General protection fault
	TRAP_GATE, // Page fault
	0, // Reserved
	TRAP_GATE, // x87 floating point exception
	TRAP_GATE, // Alignment check
	TRAP_GATE, // Machine check
	TRAP_GATE, // SIMD floating point exception
	TRAP_GATE, // Virtualization exception
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
	0, // Reserved
};

IDT64_PTR idt64_ptr = (IDT64_PTR){
		.limit = sizeof(idt64) - 1,
		.base = (uint64_t) idt64
};

void load_idt64(void)
{
	stop_interrupts();
	idt_init();
	asm volatile("lidt %0" : : "m"(idt64_ptr));
	start_interrupts();
	/*
	void (*test)(void) = (void (*)(void))(((uint64_t)idt64[5].offset_3 << 32) | ((uint64_t)idt64[5].offset_2 << 16)
		| idt64[5].offset_1);
	test();*/
}

// TODO add diffrent types of gates? not sure if needed
void idt_set_entry(int n, uint64_t handler)
{
	idt64[n] = (IDT64){
			.offset_1 = handler & 0xFFFF,
			.selector = 0x08,
			.ist = 0,
			.type_attributes = n < 32? types[n] : INTERRUPT_GATE,
			.offset_2 = (handler >> 16) & 0xFFFF,
			.offset_3 = (handler >> 32) & 0xFFFFFFFF,
			._ = 0
	};
}

void stop_interrupts(void)
{
	asm volatile("cli");
}

void start_interrupts(void)
{
	asm volatile("sti");
}