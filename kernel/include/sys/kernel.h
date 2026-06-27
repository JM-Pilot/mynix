// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/sys/kernel.c globalize all the important files
 * Copyright (C) 2026 JM-Pilot
 */

#include <multiboot.h>
#ifndef _MYNIX_KERNEL_H
#define _MYNIX_KERNEL_H

extern struct multiboot_tag *mb_tag;
extern struct console kcon;
extern struct psf2_font *kfont;

#define KERNEL_LOG_ERROR 1
#define KERNEL_LOG_DEBUG 2

#endif 