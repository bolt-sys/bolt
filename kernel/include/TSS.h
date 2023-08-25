#ifndef BOLT_TSS_H
#define BOLT_TSS_H

typedef volatile struct tss_struct
{
	unsigned short link;
	unsigned short link_h;

	unsigned long esp0;
	unsigned short ss0;
	unsigned short ss0_h;

	unsigned long esp1;
	unsigned short ss1;
	unsigned short ss1_h;

	unsigned long esp2;
	unsigned short ss2;
	unsigned short ss2_h;

	unsigned long cr3;
	unsigned long eip;
	unsigned long eflags;

	unsigned long eax;
	unsigned long ecx;
	unsigned long edx;
	unsigned long ebx;

	unsigned long esp;
	unsigned long ebp;

	unsigned long esi;
	unsigned long edi;

	unsigned short es;
	unsigned short es_h;

	unsigned short cs;
	unsigned short cs_h;

	unsigned short ss;
	unsigned short ss_h;

	unsigned short ds;
	unsigned short ds_h;

	unsigned short fs;
	unsigned short fs_h;

	unsigned short gs;
	unsigned short gs_h;

	unsigned short ldt;
	unsigned short ldt_h;

	unsigned short trap;
	unsigned short iomap;
} __attribute__((packed)) tss;

void tss_set_stack(unsigned int);
void tss_install(signed int, unsigned short, unsigned short);
extern void tss_flush();

#endif