/*
 * maths/Plane.h
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
 * Manipulation of plane equations stored in maths::Vector.
 *
 */

#ifndef _MATHS_PLANE_H_
#define _MATHS_PLANE_H_

#include <maths/Vector.h>
#include <maths/Matrix.h>
#include <util/assert.h>

namespace maths {
  /// Construct a plane from a normal and intersecting point.
  template <int N, typename T>
  Vector<N+1, T> planeFromNormal(const Vector<N,T> &n, const Vector<N,T> &pt)
  {
    return (n, -(n*pt));
  }

  /// Construct a 3d plane from points.
  template <typename T>
  Vector<4, T> planeFromPoints(const Vector<3,T> pts[3])
  {
    Vector<3, T> norm;
    cross(norm, pts[1] - pts[0], pts[2] - pts[0]);
    return planeFromNormal(norm, pts[0]);
  }

  /// Evaluate a plane equation with a homogenous point.
  template <int N, typename T>
  T plane(const Vector<N,T> &pl, const Vector<N,T> &pt)
  {
    return pl * pt;
  }

  /// Evaluate a plane equation with a non-homogenous point.
  template <int N, typename T>
  T plane(const Vector<N,T> &pl, const Vector<N-1,T> &pt)
  {
    return plane(pl, (pt, (T)1));
  }

  /// Get normal part of plane equation.
  template <int N, typename T>
  Vector<N-1,T> planeNormal(const Vector<N,T> &pl)
  {
    return pl.template slice<0,N-1>();
  }

  /// Normalise the normal part of a plane equation.
  template <int N, typename T>
  Vector<N,T> &normalisePlane(Vector<N,T> &pl)
  {
    T mag = planeNormal(pl).mag();
    RUNTIME_ASSERT(mag, "Divide by zero during normalisePlane("<<pl<<")");
    return pl /= mag;
  }

  /// Transform a plane equation by a transformation matrix.
  template <int N, typename T>
  Vector<N,T> mul(const Matrix<N,T> m, const Vector<N,T> pl)
  {
    Vector<N,T> normh = (planeNormal(pl), (T)0);
    normh = m*normh;
    Vector<N,T> pt = (-pl[3]*planeNormal(pl), (T)1);
    pt = m*pt;
    return planeFromNormal(normh.template slice<0,N-1>(), pt.template slice<0,N-1>());
  }

  /// Calculate clip planes from projection matrix.
  template <int N, typename T>
  void clipPlanes(const Matrix<N,T> clip, Vector<N,T> pl[2*(N-1)])
  {
    unsigned int i, j;

    for (i = 0; i < N-1; ++i) {
      for (j = 0; j < N; ++j) {
        pl[i*2][j]     = clip[j][N-1] - clip[j][i];
        pl[i*2 + 1][j] = clip[j][N-1] + clip[j][i];
      }
      normalisePlane(pl[i*2]);
      normalisePlane(pl[i*2 + 1]);
    }
  }
};

#endif // _MATHS_PLANE_H_
