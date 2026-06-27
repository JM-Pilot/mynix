// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/arch/x86/asm.h - provides asm functions
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#ifndef _ARCH_X86_ASM_H
#define _ARCH_X86_ASM_H

static inline void port_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
static inline void port_outw(uint16_t port, uint16_t val)
{
	__asm__ volatile ("outw %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
static inline void port_outl(uint16_t port, uint32_t val)
{
	__asm__ volatile ("outl %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
static inline uint8_t port_inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ( "inb %w1, %b0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
static inline uint16_t port_inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile ( "inw %w1, %b0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
static inline uint32_t port_inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile ( "inl %w1, %b0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
#endif /* _ARCH_X86_ASM_H */