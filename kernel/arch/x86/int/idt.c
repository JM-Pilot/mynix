// SPDX-License-Identifier: GPL-2.0-only
/* kernel/arch/x86/int/idt.c - set up the interrupt descriptor table in x86/i386 
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#include <sys/printk.h>
#include <sys/kernel.h>
#include <arch/x86/int/idt.h>
#include <arch/x86/int/pic.h>
extern uint32_t isr_stub_table[32];

__attribute__((aligned(0x10))) 
static struct idt_entry idt[256];

static struct idt_desc idtr;

void idt_set_entry(
	uint8_t n,
	uint32_t offset,
	uint16_t segment,
	uint8_t flags
)
{
	idt[n].offset_low = offset & 0xFFFF;
	idt[n].offset_high = (offset >> 16) & 0xFFFF;
	idt[n].segment = segment;
	idt[n].flags = flags;
	idt[n].rsvd = 0;
}

void idt_init()
{
	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint32_t)&idt;

	/* set all to 0*/
	for (int i = 0; i < 256; i++)
		idt_set_entry(i, 0, 0, 0);
	
	for (int i = 0; i < 32; i++)
		idt_set_entry(i, isr_stub_table[i], 0x8, 0x8E);
	__asm__ volatile ("lidt %0" :: "m"(idtr));
}

const char *exceptions[] = {
	"DIVISION ERROR",
	"DEBUG",
	"NON MASKABLE INT",
	"BREAKPOINT",
	"OVERFLOW",
	"BOUND RANGE EXCEEDED",
	"INVALID OPCODE",
	"DEVICE NOT AVAILABLE",
	"DOUBLE FAULT",
	"COPROCESSOR SEGMENT OVERRUN",
	"INVALID TSS",
	"SEGMENT NOT PRESENT",
	"STACK-SEGMENT FAULT",
	"GENERAL PROTECTION FAULT",
	"PAGE FAULT",
	"RESERVED",
	"x87 FPU EXCEPTION",
	"ALIGNMENT CHECK",
	"MACHINE CHECK",
	"SIMD FPU EXCEPTION",
	"VIRTUALIZATION EXCEPTION",
	"CONTROL PROTECTION EXCEPTION",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"RESERVED",
	"HYPERVISOR INJECTION EXCEPTION",
	"VMM COMMUNICAITON EXCEPTION",
	"SECURITY EXCEPTION",
	"RESERVED"
};
void c_isr_handler(struct interrupt_frame *iframe)
{
	if (iframe->int_num > 31)
		irq_handler(iframe);
	if (iframe->int_num < 32)
	{
		printk("\n\033[31m");
		printk("ERROR! INTERRUPT REACHED\n");
		printk("INTERRUPT: %#.2X, %s\n", iframe->int_num, exceptions[iframe->int_num]);
		printk("ERROR CODE %#.2X\n", iframe->int_err_code);
		
		printk("GS      %#.8X\n", iframe->gs);
		printk("FS      %#.8X\n", iframe->fs);
		printk("ES      %#.8X\n", iframe->es);
		printk("DS      %#.8X\n", iframe->ds);

		printk("EDI     %#.8X\n", iframe->edi);
		printk("ESI     %#.8X\n", iframe->esi);
		printk("EBP     %#.8X\n", iframe->ebp);
		printk("ESP     %#.8X\n", iframe->esp);
		printk("EBX     %#.8X\n", iframe->ebx);
		printk("EDX     %#.8X\n", iframe->edx);
		printk("ECX     %#.8X\n", iframe->ecx);
		printk("EAX     %#.8X\n", iframe->eax);

		printk("EIP     %#.8X\n", iframe->eip);
		printk("CS      %#.8X\n", iframe->cs);
		printk("EFLAGS  %#.8X\n", iframe->eflags);
		__asm__ volatile ("cli");
		for (;;) __asm__ volatile ("hlt");
	}
}