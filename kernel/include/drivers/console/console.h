// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/drivers/console/console.h globalize important functions 
 * for making a console
 * Copyright (C) 2026 JM-Pilot
 */

#include <stdint.h>
#include <psf.h>
#ifndef _MYNIX_DRIVERS_CONSOLE_H
#define _MYNIX_DRIVERS_CONSOLE_H

struct console {
	int cx, cy;
	uint32_t fg, bg;
	int cellsx, cellsy;
	struct psf2_font *font;
};

void con_init(struct console *con, struct psf2_font **font, uint32_t fg, uint32_t bg);
void con_set_col(struct console *con, uint32_t fg, uint32_t bg);
void con_putc(struct console *con, char c);
void con_puts(struct console *con, const char *str);
void con_draw_cursor(struct console *con);
#endif

