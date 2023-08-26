//
// Created by Emil Glenn Karpawitz on 8/25/23.
//

#include "isr.h"

extern void puts(char *s);

void isr_handler(registers *r)
{
	puts("recieved interrupt: ");
}