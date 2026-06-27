// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/drivers/video/fb.h - provides necessary framebuffer stuff
 * Copyright (c) 2026 JM-Pilot
 */

#include <stdint.h>
#include <multiboot.h>
#ifndef _MYNIX_DRIVERS_VIDEO_FB_H
#define _MYNIX_DRIVERS_VIDEO_FB_H

extern struct multiboot_tag_framebuffer_common fb_main;

void fb_init(struct multiboot_tag_framebuffer *fb_tag);
void fb_put_pix(uint32_t col, int x, int y);
void fb_clear(uint32_t col);
uint32_t fb_get_pixdata(int x, int y);

#endif /* _MYNIX_DRIVERS_VIDEO_FB_H */