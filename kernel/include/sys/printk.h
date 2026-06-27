// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/sys/printk.c provides formatted print output
 * Copyright (C) 2026 JM-Pilot
 */

#include <stdarg.h>
#ifndef _MYNIX_PRINTK_H
#define _MYNIX_PRINTK_H

int vsprintf(char *buf, const char *fmt, va_list args);
int printk(const char *fmt, ...);
int lprintk(int level, const char *fmt, ...);
#endif
