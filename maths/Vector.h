/*
 * maths/Vector.h
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
 * Template vector class, with both generic type and dimentions. Designed for
 * spacial coordinate vectors.
 *
 * Originally from Computer Graphics and Visualisation Open Assessment.
 *
 */

#ifndef _MATHS_VECTOR_H_
#define _MATHS_VECTOR_H_

#include <maths/TemplateMaths.h>
#include <util/assert.h>

#include <ostream>
#include <sstream>

namespace maths {

  /// Vector.
  /**
   * @param N int Number of components in the vector.
   * @param T typename Type of the components of the vector.
   */
  template <int N, typename T>
  class Vector
  {
    COMPILETIME_ASSERT(N > 0, dimentionality_too_low);
    public:
      T v[N];

      enum {
        length = N
      };
      typedef T Scalar;

      /// Default constructor (Does not initialise the values!)
      inline Vector()
      {
      }

      inline explicit Vector(T value)
      {
        for (int i = 0; i < N; ++i)
          v[i] = value;
      }
      inline Vector(T x, T y)
      {
        COMPILETIME_ASSERT(N >= 2, too_many_arguments);
        v[0] = x;
        v[1] = y;
        for (int i = 2; i < N; ++i)
          v[i] = maths::zero<T>();
      }
      inline Vector(T x, T y, T z)
      {
        COMPILETIME_ASSERT(N >= 3, too_many_arguments);
        v[0] = x;
        v[1] = y;
        v[2] = z;
        for (int i = 3; i < N; ++i)
          v[i] = maths::zero<T>();
      }
      inline Vector(T x, T y, T z, T w)
      {
        COMPILETIME_ASSERT(N >= 4, too_many_arguments);
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
        for (int i = 4; i < N; ++i)
          v[i] = maths::zero<T>();
      }
      inline explicit Vector(const T * vec)
      {
        for (int i = 0; i < N; ++i)
          v[i] = vec[i];
      }
      template <int M, typename U>
      inline explicit Vector(const Vector<M, U> & copy)
      {
        int i = 0;
        for (; i < N && i < M; ++i)
          v[i] = (T)copy.v[i];
        for (; i < N; ++i)
          v[i] = maths::zero<T>();
      }

      inline const Vector & set()
      {
        for (int i = 0; i < N; ++i)
          v[i] = maths::zero<T>();
        return *this;
      }
      inline const Vector & set(T all)
      {
        for (int i = 0; i < N; ++i)
          v[i] = all;
        return *this;
      }
      inline const Vector & set(T x, T y)
      {
        COMPILETIME_ASSERT(N >= 2, too_many_arguments);
        v[0] = x;
        v[1] = y;
        for (int i = 2; i < N; ++i)
          v[i] = maths::zero<T>();
        return *this;
      }
      inline const Vector & set(T x, T y, T z)
      {
        COMPILETIME_ASSERT(N >= 3, too_many_arguments);
        v[0] = x;
        v[1] = y;
        v[2] = z;
        for (int i = 3; i < N; ++i)
          v[i] = maths::zero<T>();
        return *this;
      }
      inline const Vector & set(T x, T y, T z, T w)
      {
        COMPILETIME_ASSERT(N >= 4, too_many_arguments);
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
        for (int i = 4; i < N; ++i)
          v[i] = maths::zero<T>();
        return *this;
      }
      inline const Vector & set(const Vector<3, T> xyz, T w)
      {
        COMPILETIME_ASSERT(N >= 4, too_many_arguments);
        v[0] = xyz[0];
        v[1] = xyz[1];
        v[2] = xyz[2];
        v[3] = w;
        for (int i = 4; i < N; ++i)
          v[i] = maths::zero<T>();
        return *this;
      }

      /// Setter for specific component.
      template <int X>
      inline void setComponent(const T newValue)
      {
        v[X] = newValue;
      }

      /// Getter for specific component.
      template <int X>
      inline T getComponent() const
      {
        return v[X];
      }

      /// Concatination operator, append a number.
      inline Vector<N+1, T> operator , (T f) const
      {
        Vector<N+1, T> result;
        for (int i = 0; i < N; ++i)
          result[i] = v[i];
        result[N] = f;
        return result;
      }

      /// Concatination operator, prepend a number.
      inline friend Vector<N+1, T> operator , (T f, const Vector & vector)
      {
        Vector<N+1, T> result;
        result[0] = f;
        for (int i = 0; i < N; ++i)
          result[i+1] = vector[i];
        return result;
      }

      /// Concatination operator, append a vector.
      template <int M>
      inline Vector<N+M, T> operator , (const Vector<M, T> & vector) const
      {
        Vector<N+M, T> result;
        for (int i = 0; i < N; ++i)
          result[i] = v[i];
        for (int i = 0; i < M; ++i)
          result[N+i] = vector[i];
        return result;
      }

      /// Get a partial vector.
      template <unsigned int S, unsigned int L>
      inline Vector<L, T> slice() const
      {
        COMPILETIME_ASSERT(S+L <= N, slice_overflow);
        return Vector<L, T>(v+S);
      }

      /// Write a partial vector.
      template <unsigned int S, unsigned int L>
      inline Vector & setSlice(const Vector<L, T> &part)
      {
        COMPILETIME_ASSERT(S+L <= N, slice_overflow);
        for (unsigned int i = 0; i < L; ++i)
          v[S+i] = part.v[i];
        return *this;
      }

      inline T & operator [] (int index)
      {
        return v[index];
      }
      inline const T & operator [] (int index) const
      {
        return v[index];
      }

      inline T * operator *()
      {
        return v;
      }
      inline const T * operator *() const
      {
        return v;
      }

      inline explicit operator T * ()
      {
        return v;
      }
      inline explicit operator const T * () const
      {
        return v;
      }

      inline Vector operator - () const
      {
        Vector ret;
        for (int i = 0; i < N; ++i)
          ret[i] = -v[i];
        return ret;
      }

      template <typename U>
      inline Vector & operator += (const Vector<N, U> & rhs)
      {
        for (int i = 0; i < N; ++i)
          v[i] += rhs.v[i];
        return *this;
      }
      template <typename U>
      inline Vector operator + (const Vector<N, U> & rhs) const
      {
        Vector ret = *this;
        return ret += rhs;
      }

      template <typename U>
      inline Vector & operator -= (const Vector<N, U> & rhs)
      {
        for (int i = 0; i < N; ++i)
          v[i] -= rhs.v[i];
        return *this;
      }
      template <typename U>
      inline Vector operator - (const Vector<N, U> & rhs) const
      {
        Vector ret = *this;
        return ret -= rhs;
      }

      inline Vector & operator *= (const T rhs)
      {
        for (int i = 0; i < N; ++i)
          v[i] *= rhs;
        return *this;
      }
      inline Vector operator * (const T rhs) const
      {
        Vector ret = *this;
        return ret *= rhs;
      }
      inline friend Vector operator * (const T lhs, const Vector & rhs)
      {
        return rhs * lhs;
      }

      /// Dot product
      inline T operator * (const Vector & rhs) const
      {
        T fDot = maths::zero<T>();
        for (int i = 0; i < N; ++i)
          fDot += v[i]*rhs.v[i];
        return fDot;
      }

      /// Dot product with a different type (uses LHS for return type)
      template <typename U>
      inline T operator * (const Vector<N,U> & rhs) const
      {
        T fDot = maths::zero<T>();
        for (int i = 0; i < N; ++i) {
          fDot += v[i]*rhs.v[i];
        }
        return fDot;
      }

      /// Dot quotient
      inline T operator / (const Vector & rhs) const
      {
        // a = B/C = (B*C)/(C*C)
        return operator * (rhs) / rhs.sqr();
      }

      inline Vector & operator /= (const T rhs)
      {
        for (int i = 0; i < N; ++i)
          v[i] /= rhs;
        return *this;
      }
      inline Vector operator / (const T rhs) const
      {
        Vector ret = *this;
        return ret /= rhs;
      }
      inline friend Vector operator / (const T lhs, const Vector & rhs)
      {
        return rhs * (lhs / rhs.sqr());
      }

      inline T sum() const
      {
        T ret = v[0];
        for (int i = 1; i < N; ++i)
          ret += v[i];
        return ret;
      }

      inline T sqr() const
      {
        return tsqr<T>();
      }
      template <typename U>
      inline U tsqr() const
      {
        U ret = maths::zero<U>();
        for (int i = 0; i < N; ++i)
          ret += maths::sqr<U>(v[i]);
        return ret;
      }

      inline Vector inv() const
      {
        return *this / sqr();
      }

      inline T mag() const
      {
        return tmag<T>();
      }
      template <typename U>
      inline U tmag() const
      {
        return maths::sqrt<U>(tsqr<U>());
      }


      inline Vector & normalize()
      {
        return operator /= (mag());
      }
      inline Vector normalized() const
      {
        return operator / (mag());
      }
      inline Vector & resize(T tLength)
      {
        return operator *= (tLength / mag());
      }
      inline Vector resized(T tLength) const
      {
        return operator * (tLength / mag());
      }

      // homogenous coordinates
      inline Vector & rehom()
      {
        COMPILETIME_ASSERT(N > 1, dimentionality_too_low_for_homogenous_coords);
        if (!v[N-1] || v[N-1] == (T)1)
          return *this;
        for (int i = 0; i < N-1; ++i)
          v[i] /= v[N-1];
        v[N-1] = (T)1;
        return *this;
      }

      // predicates

      /// Equality
      inline bool operator == (const Vector & rhs) const
      {
        for (int i = 0; i < N; ++i)
          if (v[i] != rhs.v[i])
            return false;
        return true;
      }
      /// Nonequality
      inline bool operator != (const Vector & rhs) const
      {
        for (int i = 0; i < N; ++i)
          if (v[i] != rhs.v[i])
            return true;
        return false;
      }

      /// Compare the magnitude of vectors.
      inline bool operator < (const Vector & rhs) const
      {
        return sqr() < rhs.sqr();
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline bool operator < (const T & rhs) const
      {
        return sqr() < rhs*rhs;
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline friend bool operator < (const T & lhs, const Vector & rhs)
      {
        return lhs*lhs < rhs.sqr();
      }
      /// Compare the magnitude of vectors.
      inline bool operator <= (const Vector & rhs) const
      {
        return sqr() <= rhs.sqr();
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline bool operator <= (const T & rhs) const
      {
        return sqr() <= rhs*rhs;
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline friend bool operator <= (const T & lhs, const Vector & rhs)
      {
        return lhs*lhs <= rhs.sqr();
      }
      /// Compare the magnitude of vectors.
      inline bool operator > (const Vector & rhs) const
      {
        return sqr() > rhs.sqr();
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline bool operator > (const T & rhs) const
      {
        return sqr() > rhs*rhs;
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline friend bool operator > (const T & lhs, const Vector & rhs)
      {
        return lhs*lhs > rhs.sqr();
      }
      /// Compare the magnitude of vectors.
      inline bool operator >= (const Vector & rhs) const
      {
        return sqr() >= rhs.sqr();
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline bool operator >= (const T & rhs) const
      {
        return sqr() >= rhs*rhs;
      }
      /// Compare the magnitude of the vector with a scalar magnitude.
      inline friend bool operator >= (const T & lhs, const Vector & rhs)
      {
        return lhs*lhs >= rhs.sqr();
      }

      /// Find whether the vector zero.
      inline bool zero() const
      {
        for (int i = 0; i < N; ++i)
          if (v[i])
            return false;
        return true;
      }

      /// Find whether the vector is unit length.
      inline bool normal() const
      {
        float mag = tsqr<float>();
        return mag > 0.99f && mag < 1.01f;
      }

      /// Safely convert to bool.
      inline explicit operator bool () const
      {
        return !zero();
      }

      inline operator std::string() const;
  }; // ::maths::Vector

  template <int N, typename T>
  inline Vector<N, T> & add(Vector<N, T> & dest,
                            const Vector<N, T> & lhs, const Vector<N, T> & rhs)
  {
    for (int i = 0; i < N; ++i)
      dest.v[i] = lhs.v[i] + rhs.v[i];
    return dest;
  }

  template <int N, typename T>
  inline Vector<N, T> & sub(Vector<N, T> & dest,
                            const Vector<N, T> & lhs, const Vector<N, T> & rhs)
  {
    for (int i = 0; i < N; ++i)
      dest.v[i] = lhs.v[i] - rhs.v[i];
    return dest;
  }

  template <int N, typename T>
  inline Vector<N, T> cmul(const Vector<N, T> & lhs, const Vector<N, T> & rhs)
  {
    Vector<N, T> dest;
    for (int i = 0; i < N; ++i)
      dest.v[i] = lhs.v[i] * rhs.v[i];
    return dest;
  }
  template <int N, typename T>
  inline Vector<N, T> & cmul(Vector<N, T> & dest,
                             const Vector<N, T> & lhs,
                             const Vector<N, T> & rhs)
  {
    for (int i = 0; i < N; ++i)
      dest.v[i] = lhs.v[i] * rhs.v[i];
    return dest;
  }

  template <typename T>
  inline Vector<3, T> & cross(Vector<3, T> & dest,
                              const Vector<3, T> & lhs,
                              const Vector<3, T> & rhs)
  {
    dest.v[0] = lhs.v[1]*rhs.v[2] - lhs.v[2]*rhs.v[1];
    dest.v[1] = lhs.v[2]*rhs.v[0] - lhs.v[0]*rhs.v[2];
    dest.v[2] = lhs.v[0]*rhs.v[1] - lhs.v[1]*rhs.v[0];
    return dest;
  }

  template <typename T>
  inline Vector<3, T> cross(const Vector<3, T> & lhs, const Vector<3, T> & rhs)
  {
    Vector<3, T> dest;
    dest.v[0] = lhs.v[1]*rhs.v[2] - lhs.v[2]*rhs.v[1];
    dest.v[1] = lhs.v[2]*rhs.v[0] - lhs.v[0]*rhs.v[2];
    dest.v[2] = lhs.v[0]*rhs.v[1] - lhs.v[1]*rhs.v[0];
    return dest;
  }


  /// Write a vector to an output stream.
  template <int N, typename T>
  inline std::ostream & operator << (std::ostream & out, const Vector<N, T> & v)
  {
    out << "(" << v[0];
    for (int i = 1; i < N; ++i)
      out << ", " << v[i];
    return out << ")";
  }
  /// Convert to a string using the above stream operator
  template <int N, typename T>
  inline Vector<N, T>::operator std::string() const
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  /// Basic function to create a one dimentional vector (scalar) which can then be expanded by concatination.
  template <typename T>
  inline maths::Vector<1, T> Scalar(const T a)
  {
    return maths::Vector<1, T>(a);
  }

} // ::maths

#endif // _MATHS_VECTOR_H_
