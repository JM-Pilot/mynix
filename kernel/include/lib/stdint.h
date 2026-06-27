// SPDX-License-Identifier: GPL-2.0-only
/* kernel/include/lib/stdint.h - provides the uintX and intX to simplify the kernel
 * Copyright (c) 2026 JM-Pilot
 */
#ifndef _MYNIX_LIBK_STDINT_H
#define _MYNIX_LIBK_STDINT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

#if defined(__i386__)
	typedef unsigned long long uint64_t;
	typedef signed long long int64_t;
	typedef uint32_t uintptr_t;
	typedef int32_t intptr_t;
#elif defined(__x86_64)
	typedef unsigned long uint64_t;
	typedef signed long int64_t;
	typedef uint64_t uintptr_t;
	typedef int64_t intptr_t;
#endif /* if defined(__i386__)*/

#endif /* _MYNIX_LIB_STDINT_H */