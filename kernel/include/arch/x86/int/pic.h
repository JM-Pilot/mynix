// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/arch/x86/int/pic.h - prototypes all function in pic.c
 * Copyright (c) 2026 JM-Pilot
 */
#include <stdint.h>
#include <arch/x86/int/idt.h>
#ifndef _ARCH_X86_PIC_H
#define _ARCH_X86_PIC_H

void pic_remap(int offset1, int offset2, int maskp1, int maskp2);
void pic_init();
void pic_disable();
void irq_set_mask(uint8_t irq_line);
void irq_clear_mask(uint8_t irq_line);
void irq_handler(struct interrupt_frame *reg);
void irq_uninstall_handler(int irq);
void irq_install_handler(int irq, void (*handler)(struct interrupt_frame *r));
#endif /* _ARCH_X86_PIC_H */