// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/arch/x86/gdt.h - set up the global descriptor table in x86/i386 
 * Copyright (c) 2026 JM-Pilot
 */
#include <stdint.h>
#ifndef _ARCH_X86_GDT_H
#define _ARCH_X86_GDT_H

struct gdt_desc {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));

void gdt_set_entry(uint8_t n, 
	uint32_t base, 
	uint32_t limit, 
	uint8_t access, 
	uint8_t flags);
void gdt_init();
#endif