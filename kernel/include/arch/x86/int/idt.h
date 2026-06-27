// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/arch/x86/int/idt.h - set up the interrupt descriptor table in x86/i386 
 * Copyright (c) 2026 JM-Pilot
 */
#include <stdint.h>
#ifndef _ARCH_X86_IDT_H
#define _ARCH_X86_IDT_H

struct idt_desc {
	uint16_t size;
	uint32_t offset;
}__attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t segment;
	uint8_t rsvd;
	uint8_t flags;
	uint16_t offset_high;
}__attribute__((packed));

struct interrupt_frame {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_num, int_err_code;
	uint32_t eip, cs, eflags;
} __attribute__((packed));

void idt_set_entry(
	uint8_t n,
	uint32_t offset,
	uint16_t segment,
	uint8_t flags
);

void idt_init();
#endif /* _ARCH_X86_IDT_H */