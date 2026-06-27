// SPDX-License-Identifier: GPL-2.0-only
/* kernel/drivers/video/fb.c - defines necessary framebuffer stuff
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#include <multiboot.h>
#include <drivers/video/fb.h>

struct multiboot_tag_framebuffer_common fb_main;
uint32_t *video_ptr;

void fb_init(struct multiboot_tag_framebuffer *fb_tag)
{
	fb_main = fb_tag->common;
	video_ptr = (uint32_t*)(uintptr_t)fb_main.framebuffer_addr;
}

void fb_put_pix(uint32_t col, int x, int y)
{
	/* write to video_ptr not fb_main duh */
	video_ptr[y * (fb_main.framebuffer_pitch / 4) + x] = col;
}
void fb_clear(uint32_t col)
{
	/* ima use memset later */
	for (uint32_t y = 0; y < fb_main.framebuffer_height; y++)
		for (uint32_t x = 0; x < fb_main.framebuffer_width; x++)
			fb_put_pix(col, x, y);
}

uint32_t fb_get_pixdata(int x, int y)
{
	return video_ptr[y * (fb_main.framebuffer_pitch / 4) + x];
}