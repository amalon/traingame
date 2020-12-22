/*
 * maths/TemplateMaths.h
 *
 * Copyright (C) 2007-2014 James Hogan <james@albanarts.com>
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
 * Template maths functions, some unecessary.
 *
 * Originally from Computer Graphics and Visualisation Open Assessment.
 *
 */

#ifndef _MATHS_TEMPLATEMATHS_H_
#define _MATHS_TEMPLATEMATHS_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cmath>

// Defines template functions for merging fp functions with 1 argument
#define DEFINE_FP1_TEMPLATE_FUNCTIONS_SAFE(NAME, FLOAT, DOUBLE) \
  template <typename T> \
  inline T NAME(T arg) \
  { \
    return arg.NAME(); \
  } \
  template <> inline float NAME<float>(float arg) \
  { \
    return FLOAT(arg); \
  } \
  template <> inline double NAME<double>(double arg) \
  { \
    return DOUBLE(arg); \
  }

// Defines template functions for merging fp functions with 1 argument
#define DEFINE_FP1_TEMPLATE_FUNCTIONS(NAME, FLOAT, DOUBLE) \
  template <typename T> \
  inline T NAME(T arg); \
  template <> inline float NAME<float>(float arg) \
  { \
    return FLOAT(arg); \
  } \
  template <> inline double NAME<double>(double arg) \
  { \
    return DOUBLE(arg); \
  }

// Defines template functions for merging fp functions with 2 arguments
#define DEFINE_FP2_TEMPLATE_FUNCTIONS(NAME, FLOAT, DOUBLE) \
  template <typename T> \
  inline T NAME(T arg1, T arg2); \
  template <> inline float NAME<float>(float arg1, float arg2) \
  { \
    return FLOAT(arg1, arg2); \
  } \
  template <> inline double NAME<double>(double arg1, double arg2) \
  { \
    return DOUBLE(arg1, arg2); \
  }

// Defines template functions for merging fp functions with 1 arguments, 2 ptrs
#define DEFINE_FP_V_VPP_TEMPLATE_FUNCTIONS(NAME, FLOAT, DOUBLE, LONGDOUBLE) \
  template <typename T> \
  inline void NAME(T arg1, T *arg2, T *arg3); \
  template <> inline void NAME<float>(float arg1, float *arg2, float *arg3) \
  { \
    FLOAT(arg1, arg2, arg3); \
  } \
  template <> inline void NAME<double>(double arg1, double *arg2, \
                                       double *arg3) \
  { \
    DOUBLE(arg1, arg2, arg3); \
  } \
  template <> inline void NAME<long double>(long double arg1, \
                                            long double *arg2, \
                                            long double *arg3) \
  { \
    LONGDOUBLE(arg1, arg2, arg3); \
  }

namespace maths {
  DEFINE_FP1_TEMPLATE_FUNCTIONS_SAFE(sqrt, std::sqrt, std::sqrt)

  DEFINE_FP1_TEMPLATE_FUNCTIONS(sin, std::sin, std::sin)
  DEFINE_FP1_TEMPLATE_FUNCTIONS(cos, std::cos, std::cos)
  DEFINE_FP1_TEMPLATE_FUNCTIONS(tan, std::tan, std::tan)

  DEFINE_FP1_TEMPLATE_FUNCTIONS(asin, std::asin, std::asin)
  DEFINE_FP1_TEMPLATE_FUNCTIONS(acos, std::acos, std::acos)
  DEFINE_FP1_TEMPLATE_FUNCTIONS(atan, std::atan, std::atan)

  DEFINE_FP2_TEMPLATE_FUNCTIONS(atan2, std::atan2, std::atan2)

#ifdef __GLIBC__
#if __GLIBC_PREREQ(2,1)
  DEFINE_FP_V_VPP_TEMPLATE_FUNCTIONS(sincos, ::sincosf, ::sincos, ::sincosl)
#define _MTM_SINCOS_FOUND
#endif
#endif

#ifdef _MTM_SINCOS_FOUND
#undef _MTM_SINCOS_FOUND
#else
#warning not using sincos
  template <typename T>
  inline void sincos(T x, T *s, T *c)
  {
    *s = sin(x);
    *c = cos(x);
  }
#endif

  DEFINE_FP1_TEMPLATE_FUNCTIONS(floor, std::floor, std::floor)

  // Cleanup internal preprocessor definitions.
#undef DEFINE_FP1_TEMPLATE_FUNCTIONS
#undef DEFINE_FP2_TEMPLATE_FUNCTIONS
#undef DEFINE_FP_V_VPP_TEMPLATE_FUNCTIONS

  /// Write values into a 2 dimentional vector.
  /**
   * @param T   Data type of vector components.
   * @param vec Vector pointer to array of @a T.
   * @param x   X coordinate of vector.
   * @param y   Y coordinate of vector.
   */
  template <typename T>
  inline T * setVector2(const T * vec, const T x, const T y)
  {
    vec[0] = x;
    vec[1] = y;
    return vec;
  }

  /// Write values into a 3 dimentional vector.
  /**
   * @param T   Data type of vector components.
   * @param vec Vector pointer to array of @a T.
   * @param x   X coordinate of vector.
   * @param y   Y coordinate of vector.
   * @param z   Z coordinate of vector.
   */
  template <typename T>
  inline T * setVector3(T * const vec, const T x, const T y, const T z)
  {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    return vec;
  }

  /// Write values into a 3 dimentional vector.
  /**
   * @param T   Data type of vector components.
   * @param vec Vector pointer to array of @a T.
   * @param x   X coordinate of vector.
   * @param y   Y coordinate of vector.
   * @param z   Z coordinate of vector.
   */
  template <typename T>
  inline T * setVector4(T * const vec,
                        const T x, const T y, const T z, const T w)
  {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    vec[3] = w;
    return vec;
  }


  /// Find the square of a number
  /**
   * Useful when the expression is complex and a macro should be avoided
   */
  template <typename T>
  inline T sqr(T value)
  {
    return value*value;
  }

  /// Zero in different types
  template <typename T>
  inline T zero()
  {
    return (T)0;
  }

  /// Minimum of values
  template <typename T>
  inline const T& min(const T& a, const T& b)
  {
    return (a < b) ? a : b;
  }
  template <typename T>
  inline T& min(T& a, T& b)
  {
    return (a < b) ? a : b;
  }

  /// Maximum of values
  template <typename T>
  inline const T& max(const T& a, const T& b)
  {
    return (a > b) ? a : b;
  }
  template <typename T>
  inline T& max(T& a, T& b)
  {
    return (a > b) ? a : b;
  }
}

#endif // _MATHS_TEMPLATEMATHS_H_
