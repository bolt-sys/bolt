//
// Created by Emil Karpawitz on 8/24/23.
//

#ifndef BOLT_GDT_H
#define BOLT_GDT_H
#include "limine.h"

typedef struct GlobalDescriptorTable
{
	uint16_t	limit_low;
	uint16_t 	base_low;
	uint8_t 	base_middle;
	uint8_t 	access;
	uint8_t 	granularity;
	uint8_t 	base_high;
} __attribute__((packed)) GDT;

typedef struct GlobalDescriptorTablePointer
{
    unsigned short	limit;
	uint64_t 		base;
} __attribute__((packed)) GDT_PTR;

// TODO: Implement
typedef enum GlobalDescriptorTableDescriptor
{
	CODE_READABLE 			= 0x02,
	DATA_WRITABLE 			= 0x02,

	CODE_CONFORMING 		= 0x04,
	DATA_DIRECTION_NORMAL 	= 0x00,
	DATA_DIRECTION_DOWN 	= 0x04,

	DATA_SEGMENT 			= 0x10,
	CODE_SEGMENT 			= 0x18,
	DESCRIPTOR_TSS 			= 0x00,

	RING0 					= 0x00,
	RING1 					= 0x20,
	RING2 					= 0x40,
	RING3 					= 0x60,

	PRESENT 				= 0x80,
} GDT_ACCESS;

typedef enum GlobalDescriptorTableFlag
{
	BIT64_SEGMENT 			= 0x20,
	BIT32_SEGMENT 			= 0x40,
	BIT16_SEGMENT 			= 0x00,

	GRANULARITY_1B 			= 0x00,
	GRANULARITY_4KB 		= 0x80,
} GDT_FLAG;

extern GDT gdt[6];
extern GDT_PTR gdt_ptr;

extern void gdt_flush(GDT_PTR* gdt_ptr);

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