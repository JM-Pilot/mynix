// SPDX-License-Identifier: GPL-2.0-only
/* kernel/arch/x86/gdt.c - set up the global descriptor table in x86/i386 
 * Copyright (c) 2026 JM-Pilot
 */
#include <stdint.h>
#include <arch/x86/gdt.h>

extern void gdt_load(uint16_t size, uint32_t offset);
struct gdt_entry gdt[5];

void gdt_set_entry(uint8_t n, 
	uint32_t base, 
	uint32_t limit, 
	uint8_t access, 
	uint8_t flags)
{
	/* fill up base_low/mid/high */
	gdt[n].base_low = base & 0xFFFF;
	gdt[n].base_mid = (base >> 16) & 0xFF;
	gdt[n].base_high = (base >> 24) & 0xFF;
	
	gdt[n].limit_low = limit & 0xFFFF;
	/* we want 0xC + 0xF (last F in base)*/
	gdt[n].flags = (flags & 0x0F) << 4 | (limit & 0x0F);	
	gdt[n].access = access;
}
void gdt_init()
{
	struct gdt_desc gdtr;
	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (uint32_t)&gdt;

	/* Null */
	gdt_set_entry(0, 0, 0, 0, 0);
	/* Kernel code and data */
	gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xC);
	gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xC);
	/* User code and data */
	gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xC);
	gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xC);

	/* load it */
	gdt_load(gdtr.size, gdtr.offset);
}