/*
 * util/assert.h
 *
 * Copyright (C) 2003-2014 James Hogan <james@albanarts.com>
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
 * Assertion macros
 *
 */

#ifndef _UTIL_ASSERT_H_
#define _UTIL_ASSERT_H_

#include <util/compiler.h>

#ifdef __cplusplus

namespace util
{
  namespace asserts
  {
    /*
     * Compile-time assertions
     */

    template <bool condition>
    class compileTimeAssert
    {
    };

    template <>
    class compileTimeAssert<true>
    {
    public:
      enum { ok };
    };

    // ok isn't an enumeration of compileTimeAssert<false>
#define COMPILETIME_ASSERT(COND, NAME)                                         \
    struct NAME : public ::util::asserts::compileTimeAssert<(COND)>            \
    {                                                                          \
    };                                                                         \
    enum { _ok_##NAME = NAME::ok }

    // default value will need truncation if COND is false
#define COMPILETIME_WARNING(COND, NAME)                                        \
    struct warning__##NAME                                                     \
    {                                                                          \
      void f() {                                                               \
        unsigned char __always_unused c = (256 - !!(COND));                    \
      }                                                                        \
    }

    // Sanity check the compiletime asserts don't fill up classes with rubbish
    namespace test
    {
      class DummyClass
      {
        int data;
        COMPILETIME_ASSERT(true, cta_always_true);
        COMPILETIME_WARNING(true, ctw_always_true);
      };
      COMPILETIME_ASSERT(sizeof(DummyClass) == sizeof(int),
                         DummyClass_should_be_same_size_as_int);
    }
  }
}

#else /* __cplusplus */

#define COMPILETIME_ASSERT(COND, NAME)          do {} while (0)
#define COMPILETIME_WARNING(COND, NAME)         do {} while (0)

#endif /* __cplusplus */


/*
 * Link-time assertion
 */


#define LINKTIME_ASSERT(COND, NAME)                                            \
    do {                                                                       \
      extern void NAME(void);                                                  \
      COMPILETIME_ASSERT(isconstexpr(COND), COND_should_be_const);             \
      if (!(COND))                                                             \
        NAME();                                                                \
    } while (0)




/*
 * Run-time assertions
 */

#ifdef NDEBUG

#define RUNTIME_ASSERT(COND, NAME, ...)     do {} while (0)

#else

#include <util/arch.h>
#include <util/macro_streams.h>

/** Handle a failed assertion and return true to break. */
bool handleFailedAssertion(const char* cond, const char* function,
                           const char* file, const int line, unsigned int hits,
                           int *ignore, const char* name, ...);

#define RUNTIME_ASSERT(COND, NAME, ...)                                        \
    do {                                                                       \
      if (unlikely(!(COND))) {                                                 \
        static int ignore = false;                                             \
        static unsigned int hits = 0;                                          \
        ++hits;                                                                \
        if (!ignore) {                                                         \
          MACRO_HANDLE_STREAM_ARG(ss, NAME);                                   \
          if (::handleFailedAssertion(#COND,                                   \
                                      __PRETTY_FUNCTION__, __FILE__, __LINE__, \
                                      hits, &ignore,                           \
                                      MACRO_STREAM_ARG(ss, NAME),              \
                                      ##__VA_ARGS__))                          \
            BREAKPOINT;                                                        \
        }                                                                      \
      }                                                                        \
    } while(0)

#endif

/* Common compile time asserts */
#define COMPILETIME_UNIMPLEMENTED()                                            \
    COMPILETIME_ASSERT(false, Not_implemented)

/* Common link time asserts */
#define LINKTIME_UNIMPLEMENTED()                                               \
    LINKTIME_ASSERT(false, Not_implemented)
#define LINKTIME_DEADCODE()                                                    \
    LINKTIME_ASSERT(false, Dead_code_generated)

/* Common run time asserts */
#define RUNTIME_UNIMPLEMENTED()                                                \
    RUNTIME_ASSERT(false, "Not implemented")
#define RUNTIME_DEADCODE()                                                     \
    RUNTIME_ASSERT(false, "Reached dead code")

#endif /* _UTIL_ASSERT_H_ */
