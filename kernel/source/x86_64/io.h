//
// Created by Robin Alexander Plate on 8/26/23.
//

#ifndef BOLT_KERNEL_SOURCE_X86_64_IO_H_
#define BOLT_KERNEL_SOURCE_X86_64_IO_H_

#include <stdint.h>

extern void outb(uint16_t port, uint8_t value);
extern void outw(uint16_t port, uint16_t value);
extern void outd(uint16_t port, uint32_t value);

extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint32_t ind(uint16_t port);

extern void outsb(uint16_t port, const char *addr, uint64_t count);
extern void outsw(uint16_t port, const short *addr, uint64_t count);
extern void outsd(uint16_t port, const long *addr, uint64_t count);

extern void insb(uint16_t port, const char *addr, uint64_t count);
extern void insw(uint16_t port, const short *addr, uint64_t count);
extern void insd(uint16_t port, const long *addr, uint64_t count);

extern void io_wait(uint16_t port);

#endif //BOLT_KERNEL_SOURCE_X86_64_IO_H_
