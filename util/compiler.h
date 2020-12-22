/*
 * util/compiler.h
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
 * Generic compiler abstractions and handy utilities.
 *
 */

#ifndef _UTIL_COMPILER_H_
#define _UTIL_COMPILER_H_


#define ARRAY_SIZE(x)           (sizeof(x) / sizeof((x)[0]))


#ifdef __GNUC__

#define likely(X)               __builtin_expect(!!(X), 1)
#define unlikely(X)             __builtin_expect(!!(X), 0)
#define isconstexpr(X)          __builtin_constant_p(X)

#define __pure                  __attribute__((pure))
#define __maybe_unused          __attribute__((unused))
#define __always_unused         __attribute__((unused))
#define __printf(a, b)          __attribute__((format(printf, a, b)))

#else /* __GNUC__ */

#define likely(X)               (X)
#define unlikely(X)             (X)
#define isconstexpr(X)          0

#define __pure
#define __maybe_usued
#define __always_usued
#define __printf(a, b)

#endif /* __GNUC__ */


#ifdef __cplusplus

struct deprecated_virtual;
/*
 * Put this at the end of a class to indicate that a virtual function shouldn't
 * be implemented by inheriting classes.
 */
#define DEPRECATED_VIRTUAL(SIG)                                                \
  private: virtual struct ::deprecated_virtual * SIG                           \
  {                                                                            \
    return 0;                                                                  \
  }

#endif /* __cplusplus */

#endif /* _UTIL_COMPILER_H_ */
