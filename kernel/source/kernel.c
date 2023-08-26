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
static void hcf(void) {
  asm ("cli");
  for (;;) {
	asm ("hlt");
  }
}

// TODO: move this to a better place.
void outb(uint16_t port, uint8_t value) {
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void inb(uint16_t port, uint8_t *value) {
  asm volatile ("inb %1, %0" : "=a"(*value) : "Nd"(port));
}

void putc(char c) {
  // Check if the serial port is ready to send data.
  uint8_t status;
  do {
	inb(0x3fd, &status);
  } while ((status & 0x20) == 0);

  outb(0x3f8, c);
}

void puts(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
	putc(str[i]);
  }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
	hcf();
  }

  load_gdt();

  puts("Hello, world!\n");

  // Initialize the bump allocator.
  BumpAllocator_init();

  char *ptr = malloc(1);
  *ptr = 'a';
  puts("Allocation succeeded! Value: ");
  putc(*(char*)ptr);
  puts("\n");

  realloc(ptr, 2);

  // Fetch the first framebuffer.
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  // Note: we assume the framebuffer model is RGB with 32-bit pixels.
  uint32_t *fb_ptr = framebuffer->address;

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
