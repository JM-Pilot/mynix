// SPDX-License-Identifier: GPL-2.0-only
/* kernel/main/kernel.c initializes the drivers and gives all the
 * required stuff in the kernel 
 * Copyright (C) 2026 JM-Pilot
 */

/* LIB*/
#include <stdint.h>
#include <psf.h>
#include <multiboot.h>

/* KERNEL */
#include <sys/kernel.h>
#include <sys/printk.h>

/* ARCH */
#include <arch/x86/gdt.h>
#include <arch/x86/int/idt.h>
#include <arch/x86/int/pic.h>

/* DRIVERS */
#include <drivers/video/fb.h>
#include <drivers/console/console.h>

/* must be called after kcon is initialized */
#define _KINIT(msg, fn)					\
	do { 						\
		con_puts(&kcon, msg); 			\
		fn; 					\
		con_puts(&kcon, "\033[32mOK\033[0m\n"); \
	} while (0)

struct multiboot_tag *mb_tag;
struct console kcon;
struct psf2_font *kfont;
static void k_hang()
{
	__asm__ volatile ("cli");
	for (;;) __asm__ volatile ("hlt");
}

void kernel_main(uint32_t magic, uint32_t mb_addr)
{
	__asm__ volatile("cli");
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		k_hang();
	}
	/* we get the tags */
	for (mb_tag = (struct multiboot_tag*)(mb_addr + 8);
		mb_tag->type != MULTIBOOT_TAG_TYPE_END; 
		mb_tag = (struct multiboot_tag *)((multiboot_uint8_t *)mb_tag 
                                       + ((mb_tag->size + 7) & ~7)))
	{
		switch (mb_tag->type)
		{
			/* get the tag then pass it to the framebuffer init (fb_init)*/
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				struct multiboot_tag_framebuffer *fb_tag = 
				(struct multiboot_tag_framebuffer*)mb_tag;
				fb_init(fb_tag);
				break;
			/* get the tag and pass it to the memory manager */
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				break;
		}
	}
	/* init the kfont, then pass it to con_init */
	psf_init(&kfont, &FONT_LAT2_TERM16);
	/* kernel console */
	con_init(&kcon, &kfont, 0xFFFFFF, 0);
	con_puts(&kcon, "Hello From KCON!\n");

	/* initialize gdt */
	_KINIT("Initializing GDT.. ", gdt_init());
	_KINIT("Initializing IDT.. ", idt_init());
	_KINIT("Initializing PIC.. ", pic_init());
	
	__asm__ volatile("sti");
	/* do stuff */
	while (1)
	{
		con_draw_cursor(&kcon);
		for (int i = 0; i < 199999999; i++);
	}
	k_hang();
}