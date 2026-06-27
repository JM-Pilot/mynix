// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/lib/psf.h - provides font loading for psf fonts
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#ifndef _MYNIX_LIBK_PSF_H
#define _MYNIX_LIBK_PSF_H

#define PSF2_FONT_MAGIC 0x864ab572

extern char _binary_misc_fonts_Lat2_Terminus16_psfu_start;
#define FONT_LAT2_TERM16 _binary_misc_fonts_Lat2_Terminus16_psfu_start
extern char _binary_misc_fonts_ter_118n_psfu_start;
#define FONT_TER_118N _binary_misc_fonts_ter_118n_psfu_start
struct psf2_font {
	uint32_t magic;
	uint32_t version;
	uint32_t header_sz;
	uint32_t flags;
	uint32_t glyph_cnt;
	uint32_t bytespg;
	uint32_t height;
	uint32_t width;
	char *font_data;
};

/* @param *font pointer to a *font, 
 * @param *font_data font like _binary_misc_fonts_font_psfu_start 
 */
int psf_init(struct psf2_font **font, char *font_data);
void psf_putc(struct psf2_font **font, 
	char c, 
	int x, int y, 
	uint32_t fg, uint32_t bg);

#endif /* _MYNIX_LIBK_PSF_H */