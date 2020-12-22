/*
 * util/arch.h
 *
 * Copyright (C) 2008-2014 James Hogan <james@albanarts.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details
 * (in the file called COPYING).
 *
 *
 * Generic architecture abstractions.
 *
 */

#ifndef _UTIL_ARCH_H_
#define _UTIL_ARCH_H_

#if defined(__i386) || defined(__x86_64)

#if defined(_MSC_VER)
#define BREAKPOINT      do { _asm { int 3 }; } while (0)
#elif defined(__GNUC__)
#define BREAKPOINT      do { asm volatile("int3"); } while (0)
#endif /* compiler */

#endif /* architecture */

#ifndef BREAKPOINT
#include <stdlib.h>
#define BREAKPOINT      do { abort(); } while (0)
#warning Please implement BREAKPOINT for your architecture.
#endif

#endif // _UTIL_ARCH_H_
