/*
 * util/macro_streams.h
 *
 * Copyright (C) 2010-2014 James Hogan <james@albanarts.com>
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
 * Handling C++ streams in a C compatible way in macros.
 *
 */

#ifndef _UTIL_MACRO_STREAMS_H
#define _UTIL_MACRO_STREAMS_H

#ifdef __cplusplus

#include <sstream>
#define MACRO_HANDLE_STREAM_ARG(LABEL, NAME)  std::stringstream LABEL; \
                                              LABEL << NAME
#define MACRO_STREAM_ARG(LABEL, NAME)         (LABEL.str().c_str())

#else /* __cplusplus */

#define MACRO_HANDLE_STREAM_ARG(LABEL, NAME)  do {} while (0)
#define MACRO_STREAM_ARG(LABEL, NAME)         NAME

#endif /* __cplusplus */

#endif /* _UTIL_MACRO_STREAMS_H */
