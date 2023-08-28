#include <stddef.h>
#include <limine.h>
#include <stdlib.h>

#include "x86_64/gdt.h"
#include "mem/bump_allocator.h"

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request
	framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Halt and catch fire function.
static void hcf(void)
{
	asm ("cli");
	for (;;) {
		asm ("hlt");
	}
}

#include "x86_64/io.h"
#include "x86_64/interrupt/idt.h"
#include "stdarg.h"
#include <string.h>

void puts(const char* str)
{

	size_t s = strlen(str);
	outsb(0x3f8, str, s);
}

int test(int n, ...)
{
	int rtn = 0;
	va_list ap;
	va_start(ap, n);
	for (int i = 0; i < n; i++) {
		rtn += va_arg(ap, int);
	}
	va_end(ap);
	return rtn;
}

void lool(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, ...)
{
	puts("lool\n");
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void)
{
	// Ensure we got a framebuffer.
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}

	load_gdt();
	load_idt64();
	BumpAllocator_init();

	puts("Hello World!");

	int a = test(3, 1, 2, 3);
	if (a != 6)
		puts("test failed\n");

	// Fetch the first framebuffer.
	struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

	// Note: we assume the framebuffer model is RGB with 32-bit pixels.
	uint32_t* fb_ptr = framebuffer->address;

	const int radius = 70;

	for (int j = 0; j < radius * 2; j++) {
		for (int k = 0; k < radius * 2; k++) {
			if ((j - radius) * (j - radius) + (k - radius) * (k - radius) < radius * radius) {
				fb_ptr[(j) * (framebuffer->pitch / 4) + k] = 0xffffff;
			}
		}
	}

	// We're done, just hang...
	hcf();
}
