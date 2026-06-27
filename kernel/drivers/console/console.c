// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/drivers/console/console.h globalize important functions 
 * for making a console
 * Copyright (C) 2026 JM-Pilot
 */
#include <stdint.h>
#include <psf.h>
#include <drivers/video/fb.h>
#include <drivers/console/console.h>

/* yes, its for readablity */
#define CON_TAB_SZ 8
void con_init(struct console *con, 
	      struct psf2_font **font, 
	      uint32_t fg, uint32_t bg)
{
	con->font = *font;
	con->fg = fg;
	con->bg = bg;
	con->cx = 0;
	con->cy = 0;
	con->cellsx = fb_main.framebuffer_width / con->font->width;
	con->cellsy = fb_main.framebuffer_height / con->font->height;
}
void con_set_col(struct console *con, uint32_t fg, uint32_t bg)
{
	con->fg = fg;
	con->bg = bg;
}

void insert_nl(struct console *con)
{
	con->cx = 0;
	if (con->cy++ >= con->cellsy) {
		con->cy--;
	}
}
void con_draw_cursor(struct console *con)
{
	/* we draw a block 
	 * probably replace this if its ugly
	 */
	uint32_t px = con->cx * con->font->width;
	uint32_t py = con->cy * con->font->height;
	for (uint32_t y = 0; y < con->font->height; y++) {
		for (uint32_t x = 0; x < con->font->width; x++) {
			uint32_t pix = fb_get_pixdata(px + x, py + y);
			pix ^= 0x00FFFFFF;
			fb_put_pix(pix, px + x, py + y);
		}
	}
}
void con_putc(struct console *con, char c)
{
	/* delete previous cursor */
	con_draw_cursor(con);
	switch (c)
	{
		case '\n':
			insert_nl(con);
			break;
		case '\r':
			con->cx = 0;
			break;
		case '\b':
			if (--con->cx < 0 && con->cy) {
				con->cy--;
			}
			break;
		case '\t':
			con->cx += CON_TAB_SZ - (con->cx % CON_TAB_SZ);
			break;
		default:
			psf_putc(&con->font, c, 
				con->cx * con->font->width, 
				con->cy * con->font->height, 
				con->fg, con->bg);
			if (con->cx++ >= con->cellsx)
				insert_nl(con);
			break;
	}
	
	con_draw_cursor(con); // draw now
}


void con_switch_col(struct console *con, int col)
{
	switch (col)
	{
		case 30: con->fg = 0x000000; break; // black
		case 31: con->fg = 0xFF0000; break; // red
		case 32: con->fg = 0x00FF00; break; // green
		case 33: con->fg = 0xFFFF00; break; // yellow
		case 34: con->fg = 0x0000FF; break; // blue
		case 35: con->fg = 0xFF00FF; break; // magenta
		case 36: con->fg = 0x00FFFF; break; // cyan
		case 37: con->fg = 0xFFFFFF; break; // white
		/* bg */
		case 40: con->bg = 0x000000; break; // black bg
		case 41: con->bg = 0xFF0000; break; // red bg
		case 42: con->bg = 0x00FF00; break; // green bg
		case 43: con->bg = 0xFFFF00; break; // yellow bg
		case 44: con->bg = 0x0000FF; break; // blue bg
		case 45: con->bg = 0xFF00FF; break; // magenta bg
		case 46: con->bg = 0x00FFFF; break; // cyan bg
		case 47: con->bg = 0xFFFFFF; break; // white bg
		break;
		default :
			con->fg = 0xFFFFFF;
			con->bg = 0;
			break;
	}
}
void con_puts(struct console *con, const char *str)
{
	while (*str) {
		if (*str == '\033') {
			str++;

			if (*str == '[') {
				str++;

				uint32_t num = 0;
			
				while (*str >= '0' && *str <= '9') {
					num = num * 10 + (*str - '0');
					str++;
				}
				if (*str == 'm') {
					con_switch_col(con, num);
					str++;
					continue;
				}
			}
		}
		/* draw the char */
		con_putc(con, *str++);
	}
}

