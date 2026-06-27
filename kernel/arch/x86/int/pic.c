// SPDX-License-Identifier: GPL-2.0-only
/* kernel/arch/x86/int/pic.c - controlls the i8259 PIC
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#include <arch/x86/int/idt.h>
#include <arch/x86/asm.h>

#define PIC1 		0x20
#define PIC2 		0xA0
/* PICX_COM is useless, use PIC1 & PIC2*/
#define PIC1_DATA 	PIC1 + 1
#define PIC2_DATA 	PIC2 + 1
#define PIC_EOI		0x20

void pic_set_eoi(uint8_t irq)
{
	if (irq >= 8)
		port_outb(PIC2, PIC_EOI);
	port_outb(PIC1, PIC_EOI);
}

#define ICW1_ICW4	0x01	
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	0x08
#define ICW1_INIT	0x10

#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

#define CASCADE_IRQ 2

void pic_remap(int offset1, int offset2, int maskp1, int maskp2)
{
	/* init cascade */
	port_outb(PIC1, ICW1_INIT | ICW1_ICW4);
	port_outb(PIC2, ICW1_INIT | ICW1_ICW4);
	/* ICW2 MASTER VECTOR OFFSET */
	port_outb(PIC1_DATA, offset1);
	/* ICW2 SLAVE VECTOR OFFSET */
	port_outb(PIC2_DATA, offset2);

	/* Tell there is a slave pic at irq2 */
	port_outb(PIC1_DATA, 1 << CASCADE_IRQ);
	/* Give it its cascade identity*/
	port_outb(PIC2_DATA, 2);
	
	/* use 8086 mode */
	port_outb(PIC1_DATA, ICW4_8086);
	port_outb(PIC2_DATA, ICW4_8086);

	port_outb(PIC1_DATA, maskp1);
	port_outb(PIC2_DATA, maskp2);
}


extern uint32_t irq_stub_table[16];
void pic_init()
{
	/* use the PIT only */
	pic_remap(0x20, 0x28, 0xFE, 0xFF);
	for (int i = 0; i < 16; i++)
		idt_set_entry(32 + i, irq_stub_table[i], 0x8, 0x8E);
}

void pic_disable() 
{
    	port_outb(PIC1_DATA, 0xFF);
    	port_outb(PIC2_DATA, 0xFF);
}

void irq_set_mask(uint8_t irq_line) 
{
	uint16_t port;
	uint8_t value;

	if(irq_line < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq_line -= 8;
	}
	value = port_inb(port) | (1 << irq_line);
	port_outb(port, value);        
}

void irq_clear_mask(uint8_t irq_line) 
{
	uint16_t port;
	uint8_t value;

	if(irq_line < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq_line -= 8;
	}
	value = port_inb(port) & ~(1 << irq_line);
	port_outb(port, value);        
}

void *irq_handlers[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *r))
{
	irq_handlers[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
	irq_handlers[irq] = 0;
}

void irq_handler(struct interrupt_frame *reg)
{
	void (*handler)(struct interrupt_frame *r) = irq_handlers[reg->int_num - 32];
	if (handler) 
		handler(reg);
	pic_set_eoi((uint8_t)reg->int_num);
}


