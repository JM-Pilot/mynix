// SPDX-License-Identifier: GPL-2.0-only
/* kernel/lib/psf.c - defines font loading functions for psf fonts
 * Copyright (c) 2026 JM-Pilot
 */
#include <stdint.h>
#include <psf.h>
#include <drivers/video/fb.h>
int psf_init(struct psf2_font **font, char *font_data)
{
	*font = (struct psf2_font*)font_data;
	(*font)->font_data = font_data;
	if ((*font)->magic != PSF2_FONT_MAGIC)
	{
		return -1;
	}
	return 0;
}
void psf_putc(struct psf2_font **font,
	char c,
	int x, int y, 
	uint32_t fg, uint32_t bg)
{
	uint8_t *glyph = (uint8_t*)(*font)->font_data + (*font)->header_sz + c * (*font)->bytespg;

	uint32_t bpg_line = ((*font)->width + 7) / 8;
	for (uint32_t gy = 0; gy < (*font)->height; gy++){
		uint8_t* current_ln = glyph + (bpg_line * gy);
		for (uint32_t gx = 0; gx < (*font)->width; gx++){
			uint8_t byte = current_ln[gx / 8];
			if (byte & (0x80 >> (gx % 8))) {
				fb_put_pix(fg, x + gx, y + gy);
			} else {
				fb_put_pix(bg, x + gx, y + gy);
			}
		}
	}
}