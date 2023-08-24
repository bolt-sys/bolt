//
// Created by Emil Karpawitz on 8/24/23.
//

#ifndef BOLT_GDT_H
#define BOLT_GDT_H

typedef struct GlobalDescriptorTable
{
	unsigned short	limit_low;
	unsigned short	base_low;
	unsigned char	base_middle;
	unsigned char	access;
	unsigned char	granularity;
	unsigned char	base_high;
} __attribute__((packed)) GDT;

typedef struct GlobalDescriptorTablePointer
{
	unsigned short	limit;
	void* 			base;
} __attribute__((packed)) GDT_PTR;

extern GDT gdt[6];
extern GDT_PTR gdt_ptr;

void gdt_flush();

/**
 * \brief Sets a new descriptor in the Global Descriptor Table (GDT)
 *
 * This function sets a new descriptor in the GDT based on the provided parameters.
 *
 * \param num     The index of the descriptor in the GDT
 * \param base    The base address of the segment
 * \param limit   The limit of the segment
 * \param access  The access byte describing characteristics of the segment
 * \param gran    The granularity byte describing the size of the segment
 */

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

/**
 * \brief Installs the GDT
 *
 * This function installs the GDT by setting the GDTR register and reloading the segment registers.
 */
void gdt_install();

#endif //BOLT_GDT_H
