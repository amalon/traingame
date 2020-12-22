/*
 * maths/Line.h
 *
 * Copyright (C) 2015 James Hogan <james@albanarts.com>
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
 * Various line representations
 *
 */

#ifndef _MATHS_LINE_H_
#define _MATHS_LINE_H_

#include <maths/Vector.h>

#include <limits>

namespace maths
{
  /* Overall wrapper */
  template <unsigned int N, typename T, typename Rep>
  class Line : public Rep::template S<N, T>::Type
  {
    protected:
      typedef typename Rep::template S<N, T>::Type Base;
    public:
      Line()
      {
      }

      template <typename B>
      explicit Line(const Line<N, T, B> &other)
      : Base(other)
      {
      }
      template <typename B>
      Line &operator =(const Line<N, T, B> &other)
      {
        Base::operator =(other);
        return *this;
      }

      maths::Vector<N, T> normal_mag(T & mag) const
      {
        if (Base::efficient_mag || Base::efficient_normal) {
          mag = Base::mag();
          return Base::normal();
        } else {
          maths::Vector<N, T> vec = Base::vector();
          mag = vec.mag();
          return vec / mag;
        }
      }

      /* interpolate along the line */
      maths::Vector<N, T> operator *(T p) const
      {
        return Base::startPosition() + Base::vector() * p;
      }

      /* find the distance along the line of a point */
      T closestPoint(const maths::Vector<N, T> &pos) const
      {
        if (Base::efficient_normal && Base::efficient_mag)
          return ((pos - Base::startPosition()) * Base::normal()) / Base::mag();
        else
          return (pos - Base::startPosition()) / Base::vector();
      }

      /* find the distance along the line of a point relative to start */
      T closestPointRel(const maths::Vector<N, T> &rel) const
      {
        if (Base::efficient_normal && Base::efficient_mag)
          return (rel * Base::normal()) / Base::mag();
        else
          return rel / Base::vector();
      }

      /* find the distance along the line of closest point to another line */
      template <typename B>
      T closestPoint(const Line<N, T, B> &other, T *otherResult = NULL) const
      {
        // Quick calculate the normals of the lines.
        maths::Vector<N, T> norm1 = Base::vector();
        maths::Vector<N, T> norm2 = other.vector();
        maths::Vector<N, T> startDiff = Base::startPosition() - other.startPosition();

        T a = norm1*norm1;
        T b = norm1*norm2;
        T c = norm2*norm2;
        T d = norm1*startDiff;
        T e = norm2*startDiff;

        // Return NAN's if the lines are parallel
        T denominator = a*c - b*b;
        if (denominator == 0.0f) {
          if (otherResult)
            *otherResult = std::numeric_limits<float>::quiet_NaN();
          return std::numeric_limits<float>::quiet_NaN();
        }

        // Otherwise do normal calculation
        if (otherResult)
          *otherResult = (a*e - b*d) / denominator;
        return (b*e - c*d) / denominator;
      }
  };

  /*
   * Multiple ways of representing lines.
   */

  template <unsigned int N, typename T>
  class _LineBase
  {
    protected:
      enum {
        dim = N,
      };
      typedef T Scalar;

    public:
      maths::Vector<N, T> start;

      _LineBase()
      {
      }

      template <typename B>
      explicit _LineBase(const Line<N, T, B> &other)
      : start(other.startPosition())
      {
      }
      template <typename B>
      _LineBase &operator =(const Line<N, T, B> &other)
      {
        start = other.startPosition();
        return *this;
      }

      const maths::Vector<N, T> &startPosition() const
      {
        return start;
      }
  };

  template <unsigned int N, typename T>
  class _LineBaseZero
  {
    protected:
      enum {
        dim = N,
      };
      typedef T Scalar;

    public:
      _LineBaseZero()
      {
      }

      template <typename B>
      explicit _LineBaseZero(const Line<N, T, B> &other)
      {
      }
      template <typename B>
      _LineBaseZero &operator =(const Line<N, T, B> &other)
      {
        return *this;
      }

      maths::Vector<N, T> startPosition() const
      {
        return maths::Vector<N, T>((T)0);
      }
  };

  template <typename Base>
  class _LineEnd : public Base
  {
    protected:
      enum {
        N = Base::dim,
      };
      typedef typename Base::Scalar T;

    public:
      enum {
        efficient_end = 1,
        efficient_vector = 0,
        efficient_normal = 0,
        efficient_mag = 0,
        efficient_sqr = 0,
      };

      maths::Vector<N, T> end;

      _LineEnd()
      {
      }

      template <typename B>
      explicit _LineEnd(const Line<N, T, B> &other)
      : Base(other),
        end(other.endPosition())
      {
      }
      template <typename B>
      _LineEnd &operator =(const Line<N, T, B> &other)
      {
        Base::operator =(other);
        end = other.endPosition();
        return *this;
      }

      const maths::Vector<N, T> &endPosition() const
      {
        return end;
      }

      /* Potentially non-normalised vector */
      maths::Vector<N, T> vector() const
      {
        return endPosition() - Base::startPosition();
      }

      /* Normalised vector */
      maths::Vector<N, T> normal() const
      {
        return vector().normalized();
      }

      T mag() const
      {
        return vector().mag();
      }

      T sqr() const
      {
        return vector().sqr();
      }
  };

  template <typename Base>
  class _LineVec : public Base
  {
    protected:
      enum {
        N = Base::dim,
      };
      typedef typename Base::Scalar T;

    public:
      enum {
        efficient_end = 0,
        efficient_vector = 1,
        efficient_normal = 0,
        efficient_mag = 0,
        efficient_sqr = 0,
      };

      maths::Vector<N, T> vec;

      _LineVec()
      {
      }

      template <typename B>
      explicit _LineVec(const Line<N, T, B> &other)
      : Base(other),
        vec(other.vector())
      {
      }
      template <typename B>
      _LineVec &operator =(const Line<N, T, B> &other)
      {
        Base::operator =(other);
        vec = other.vector();
        return *this;
      }

      maths::Vector<N, T> endPosition() const
      {
        return Base::startPosition() + vec;
      }

      /* Potentially non-normalized vector */
      const maths::Vector<N, T> &vector() const
      {
        return vec;
      }

      /* Normalised vector */
      maths::Vector<N, T> normal() const
      {
        return vector().normalized();
      }

      T mag() const
      {
        return vector().mag();
      }

      T sqr() const
      {
        return vector().sqr();
      }
  };

  template <typename Base>
  class _LineUnit : public Base
  {
    protected:
      enum {
        N = Base::dim,
      };
      typedef typename Base::Scalar T;

    public:
      enum {
        efficient_end = 0,
        efficient_vector = 1,
        efficient_normal = 1,
        efficient_mag = 1,
        efficient_sqr = 1,
      };

      maths::Vector<N, T> norm;

      _LineUnit()
      {
      }

      template <typename B>
      _LineUnit(const Line<N, T, B> &other, bool dummy)
      : Base(other)
      {
      }

      template <typename B>
      explicit _LineUnit(const Line<N, T, B> &other)
      : Base(other),
        norm(other.normal())
      {
      }
      template <typename B>
      _LineUnit &operator =(const Line<N, T, B> &other)
      {
        Base::operator =(other);
        norm = other.normal();
        return *this;
      }

      maths::Vector<N, T> endPosition() const
      {
        return Base::startPosition() + norm;
      }

      /* Potentially non-normalized vector */
      const maths::Vector<N, T> &vector() const
      {
        return norm;
      }

      /* Normalised vector */
      const maths::Vector<N, T> &normal() const
      {
        return norm;
      }

      T mag() const
      {
        return (T)1;
      }

      T sqr() const
      {
        return (T)1;
      }

      maths::Vector<N, T> interp(T p) const
      {
        return Base::startPosition() + vector() * p;
      }
  };

  template <typename BASE>
  class _LineNorm : public _LineUnit<BASE>
  {
    protected:
      typedef _LineUnit<BASE> Base;
      enum {
        N = Base::dim,
      };
      typedef typename Base::Scalar T;

    public:
      T len;

      _LineNorm()
      {
      }

      template <typename B>
      explicit _LineNorm(const Line<N, T, B> &other)
      : Base(other, true)
      {
        Base::norm = other.normal_mag(len);
      }
      template <typename B>
      _LineNorm &operator =(const Line<N, T, B> &other)
      {
        Base::Base::operator =(other);
        Base::norm = other.normal_mag(len);
        return *this;
      }

      maths::Vector<N, T> endPosition() const
      {
        return Base::startPosition() + Base::normal() * len;
      }

      /* Potentially non-normalized vector */
      maths::Vector<N, T> vector() const
      {
        return Base::normal() * len;
      }

      T mag() const
      {
        return len;
      }

      T sqr() const
      {
        return len * len;
      }
  };

  struct LineEnds { template <unsigned int N, typename T> struct S {
    typedef _LineEnd<_LineBase<N, T> > Type;
  }; };
  struct LineVector { template <unsigned int N, typename T> struct S {
    typedef _LineVec<_LineBase<N, T> > Type;
  }; };
  struct LineNormal { template <unsigned int N, typename T> struct S {
    typedef _LineNorm<_LineBase<N, T> > Type;
  }; };
  struct LineUnit { template <unsigned int N, typename T> struct S {
    typedef _LineUnit<_LineBase<N, T> > Type;
  }; };

  struct LineEnd { template <unsigned int N, typename T> struct S {
    typedef _LineEnd<_LineBaseZero<N, T> > Type;
  }; };
  struct LineOriginVector { template <unsigned int N, typename T> struct S {
    typedef _LineVec<_LineBaseZero<N, T> > Type;
  }; };
  struct LineOriginNormal { template <unsigned int N, typename T> struct S {
    typedef _LineNorm<_LineBaseZero<N, T> > Type;
  }; };
  struct LineOriginUnit { template <unsigned int N, typename T> struct S {
    typedef _LineUnit<_LineBaseZero<N, T> > Type;
  }; };
}

#endif // _MATHS_LINE_H_
