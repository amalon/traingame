/*
 * maths/Matrix.h
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
 * Square transformation matrix class.
 *
 * Originally from Computer Graphics and Visualisation Open Assessment.
 *
 */

#ifndef _MATHS_MATRIX_H_
#define _MATHS_MATRIX_H_

#include <maths/Vector.h>
#include <maths/TemplateMaths.h>
#include <util/assert.h>

// For memcpy
#include <cstring>
#include <ostream>
#include <sstream>

namespace maths
{
  // This isn't always used with matricies.
  // Include maths/Quaternion.h if you wish to use it.
  template <typename T>
      class Quaternion;

  template <int N, typename T>
      class Matrix;

  /// Get an arbitrary matrix where the z axis points in the direction of Z.
  Matrix<3,float> getTbnMatrix(Vector<3, float> vZ);

  Matrix<4,float> RotateRadMatrix44(char axis, float rad);

  /// Square tranformation matrix.
  /**
   * The matrix is stored in a column major fashion so as to be directly.
   *  compatible with OpenGL
   * @param N int Size of width and height of the matrix.
   * @param T typename Type of the components of the matrix.
   */
  template <int N, typename T>
      class Matrix
  {
    COMPILETIME_ASSERT(N > 0, dimentionality_too_low);
    public:
      /// Columns
      Vector<N, T> col[N];

      /// Default constructor (does not initialise values).
      inline Matrix()
      {
        // let the vector constructor deal with the initialisation
      }

      /// Diagonal matrix constructor
      inline explicit Matrix(T diagonal)
      {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < i; ++j) {
            col[i][j] = col[j][i] = maths::zero<T>();
          }
          col[i][i] = diagonal;
        }
      }

      /// Diagonal matrix constructor
      inline explicit Matrix(Vector<N, T> diagonal)
      {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < i; ++j) {
            col[i][j] = col[j][i] = maths::zero<T>();
          }
          col[i][i] = diagonal[i];
        }
      }

      inline Matrix(Vector<N, T> x, Vector<N, T> y)
      {
        col[0] = x;
        col[1] = y;
        if (N > 2) {
          for (int i = 2; i < N; ++i) {
            col[i] = (Vector<N, T>)maths::zero<T>();
          }
        }
      }
      inline Matrix(Vector<N, T> x, Vector<N, T> y, Vector<N, T> z)
      {
        col[0] = x;
        col[1] = y;
        if (N > 2) {
          col[2] = z;
          if (N > 3) {
            for (int i = 3; i < N; ++i) {
              col[i] = (Vector<N, T>)maths::zero<T>();
            }
          }
        }
      }
      inline Matrix(Vector<N, T> x, Vector<N, T> y, Vector<N, T> z, Vector<N, T> w)
      {
        col[0] = x;
        col[1] = y;
        if (N > 2) {
          col[2] = z;
          if (N > 3) {
            col[3] = w;
            if (N > 4) {
              for (int i = 4; i < N; ++i) {
                col[i] = (Vector<N, T>)maths::zero<T>();
              }
            }
          }
        }
      }

      /// Copy the matrix directly from an array.
      /**
       * @param vec Pointer to array of values in column major format.
       */
      inline explicit Matrix(const T * vec)
      {
        std::memcpy(&col[0][0], vec, sizeof(T)*(N*N));
      }

      /// Copy the matrix from an array of vectors.
      /**
       * @param columns Pointer to array of column vectors.
       */
      inline explicit Matrix(const Vector<N, T> * columns)
      {
        std::memcpy(col, columns, sizeof(T)*(N*N));
      }
      /// Convert quaternion to matrix
      explicit Matrix(const Quaternion<T> &);

      /// From something else
      /**
       * Basically we want to prevent accidental double conversions between types.
       */
      template <typename U>
          explicit Matrix(const U &);

      /// Convert, downsize or upsize another matrix.
      /**
       * If the matrix is being downsized, the extreme rows and columns are
       *  ignored.
       * If the matrix is being upsized, the extreme rows and columns are filled
       *  with the identity matrix.
       * @param copy The matrix object to construct from.
       */
      template <int M, typename U> inline explicit Matrix(const Matrix<M,U> & copy)
      {
        const int copyRange = (N < M ? N : M);
        // Copy as many columns as possible from copy
        int i = 0;
        for (; i < copyRange; ++i) {
          // Copy as many values as possible from the column
          int j = 0;
          for (; j < copyRange; ++j)
            col[i][j] = (T)copy.col[i][j];
          if (M < N) {
            // Generate any remaining values from the identity matrix.
            for (; j < N; ++j)
              col[i][j] = (i==j ? (T)1 : maths::zero<T>());
          }
        }
        if (M < N) {
          // Generate any remaining columns from the identity matrix.
          for (; i < N; ++i) {
            for (int j = 0; j < N; ++j)
              col[i][j] = (i==j ? (T)1 : maths::zero<T>());
          }
        }
      }

      /// Index a column in the matrix.
      /**
       * @param columnIndex The index of the column starting at 0.
       * @return The column as a vector.
       */
      inline Vector<N, T> & operator [] (int columnIndex)
      {
        return col[columnIndex];
      }
      /// Index a column in the matrix.
      /**
       * @param columnIndex The index of the column starting at 0.
       * @return The column as a vector.
       */
      inline const Vector<N, T> & operator [] (int columnIndex) const
      {
        return col[columnIndex];
      }

      /// Get a pointer to the values of the matrix.
      /**
       * @return Pointer to the values of the array in column major format.
       */
      inline operator T * ()
      {
        return (T*)col;
      }
      /// Get a pointer to the values of the matrix.
      /**
       * @return Pointer to the values of the array in column major format.
       */
      inline operator const T * () const
      {
        return (const T*)col;
      }

      inline Matrix & operator += (const Matrix & rhs)
      {
        for (int i = 0; i < N; ++i)
          col[i] += rhs.col[i];
        return *this;
      }
      inline Matrix operator + (const Matrix & rhs) const
      {
        Matrix ret = *this;
        return ret += rhs;
      }

      inline Matrix & operator -= (const Matrix & rhs)
      {
        for (int i = 0; i < N; ++i)
          col[i] -= rhs.col[i];
        return *this;
      }
      inline Matrix operator - (const Matrix & rhs) const
      {
        Matrix ret = *this;
        return ret -= rhs;
      }

      inline Matrix & operator *= (const T rhs)
      {
        for (int i = 0; i < N; ++i)
          col[i] *= rhs;
        return *this;
      }
      inline Matrix operator * (const T rhs) const
      {
        Matrix ret = *this;
        return ret *= rhs;
      }

      inline friend Matrix operator * (const T lhs, const Matrix & rhs)
      {
        return rhs*lhs;
      }

      template <typename U>
      inline friend Vector<N,U> operator * (const Matrix & lhs, const Vector<N,U> & rhs)
      {
        Vector<N,U> ret;
        for (int row = 0; row < N; ++row) {
          ret[row] = maths::zero<U>();
          for (int i = 0; i < N; ++i) {
            ret[row] += lhs[i][row] * rhs[i];
          }
        }
        return ret;
      }
      template <typename U>
      inline friend Vector<N,U> operator * (const Vector<N,U> & lhs, const Matrix & rhs)
      {
        Vector<N,U> ret;
        for (int col = 0; col < N; ++col) {
          ret[col] = maths::zero<T>();
          for (int i = 0; i < N; ++i) {
            ret[col] += lhs[i] * rhs[col][i];
          }
        }
        return ret;
      }

      inline friend Matrix operator * (const Matrix & lhs, const Matrix & rhs)
      {
        Matrix ret;
        for (int col = 0; col < N; ++col) {
          for (int row = 0; row < N; ++row) {
            T val = maths::zero<T>();
            for (int i = 0; i < N; ++i) {
              val += lhs[i][row] * rhs[col][i];
            }
            ret[col][row] = val;
          }
        }
        return ret;
      }
      inline Matrix & operator *= (const Matrix & rhs)
      {
        return (*this = *this * rhs);
      }
      inline Matrix & premul(const Matrix & lhs)
      {
        return (*this = lhs * *this);
      }

      inline Matrix & operator /= (const T rhs)
      {
        for (int i = 0; i < N; ++i)
          col[i] /= rhs;
        return *this;
      }
      inline Matrix operator / (const T rhs) const
      {
        Matrix ret = *this;
        return ret /= rhs;
      }

      /// Make the matrix an identity matrix.
      inline Matrix & setIdentity()
      {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = 1;
        }
        return *this;
      }
      inline static const Matrix & identity()
      {
        static const Matrix i(1.0f);
        return i;
      }

      /// Make the matrix a scale matrix.
      inline Matrix & setScale(const T &v)
      {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = v;
        }
        return *this;
      }
      inline Matrix & setScale(const maths::Vector<N, T> &v)
      {
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = v[i];
        }
        return *this;
      }
      template <int NN>
      inline Matrix & setScale(const maths::Vector<NN, T> &v)
      {
        int i;
        for (i = 0; i < maths::min(N,NN); ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = v[i];
        }
        for (; i < N; ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = (T)1;
        }
        return *this;
      }
      template <typename TT>
      inline static Matrix scale(const TT &v)
      {
        Matrix ret;
        ret.setScale(v);
        return ret;
      }

      /// Make the matrix a translation matrix.
      inline Matrix & setTranslation(const maths::Vector<N-1, T> &v)
      {
        for (int i = 0; i < N-1; ++i) {
          for (int j = 0; j < i; ++j)
            col[i][j] = col[j][i] = maths::zero<T>();
          col[i][i] = (T)1;
          col[i][N-1] = maths::zero<T>();
        }
        col[N-1].template setSlice<0,N-1>(v);
        col[N-1][N-1] = (T)1;
        return *this;
      }
      inline static Matrix translation(const maths::Vector<N-1,T> &v)
      {
        Matrix ret;
        ret.setTranslation(v);
        return ret;
      }

      /// Make the matrix a rotation matrix.
      Matrix & setRotation(T theta)
      {
        COMPILETIME_ASSERT(N >= 2, dimentionality_too_low);

        T sinA, cosA;
        maths::sincos(theta, &sinA, &cosA);
        col[0][0] = cosA;
        col[0][1] = -sinA;
        col[1][0] = sinA;
        col[1][1] = cosA;

        // fill the rest of the matrix from identity
        for (int i = 1; i < N; ++i) {
          for (int j = 0; j < i; ++j) {
            col[i][j] = maths::zero<T>();
            col[j][i] = maths::zero<T>();
          }
          col[i][i] = (T)1;
        }

        return *this;
      }
      Matrix & setRotation(const maths::Vector<3, T>&axis, T theta)
      {
        COMPILETIME_ASSERT(N >= 3, dimentionality_too_low);
        /* axis is assumed to be normalised */
        RUNTIME_ASSERT(axis.sqr() > 0.99f && axis.sqr() < 1.01f,
                       "setRotation axis not normalised"
                       << ", axis=" << axis
                       << ", theta=" << theta);

        T sinA, cosA;
        maths::sincos(theta, &sinA, &cosA);
        const T invCosA = (T)1 - cosA;

        maths::Vector<3,T> nn(invCosA * axis);
        const T xy = nn[0] * axis[1];
        const T xz = nn[0] * axis[2];
        const T yz = nn[1] * axis[2];
        maths::cmul(nn, nn, axis);

        const maths::Vector<3,T> sinAaxis(sinA * axis);

        col[0][0] = nn[0] + cosA;
        col[0][1] = xy + sinAaxis[2];
        col[0][2] = xz - sinAaxis[1];

        col[1][0] = xy - sinAaxis[2];
        col[1][1] = nn[1] + cosA;
        col[1][2] = yz + sinAaxis[0];

        col[2][0] = xz + sinAaxis[1];
        col[2][1] = yz - sinAaxis[0];
        col[2][2] = nn[2] + cosA;

        // fill the rest of the matrix from identity
        for (int i = 3; i < N; ++i) {
          for (int j = 0; j < i; ++j) {
            col[i][j] = maths::zero<T>();
            col[j][i] = maths::zero<T>();
          }
          col[i][i] = (T)1;
        }

        /* Result should really be orthonormal, otherwise we've done something
         * wrong */
        RUNTIME_ASSERT(orthonormal(),
                       "setRotation result not orthonormal"
                       << ", axis=" << axis
                       << ", theta=" << theta
                       << ", result=" << *this);
        return *this;
      }
      inline static Matrix rotation(T theta)
      {
        Matrix ret;
        ret.setRotation(theta);
        return ret;
      }
      template <int NN>
      inline static Matrix rotation(const maths::Vector<NN,T> &axis, T theta)
      {
        Matrix ret;
        ret.setRotation(axis, theta);
        return ret;
      }

      /// Transpose the matrix.
      inline Matrix & transpose()
      {
        // go through lower left cells and swap with upper right
        for (int i = 1; i < N; ++i) {
          for (int j = 0; j < i; ++j) {
            T temp = col[i][j];
            col[i][j] = col[j][i];
            col[j][i] = temp;
          }
        }
        return *this;
      }

      /// Get the transposed matrix.
      inline Matrix transposed() const
      {
        Matrix ret;
        for (int i = 0; i < N; ++i) {
          for (int j = 0; j < N; ++j) {
            ret[j][i] = col[i][j];
          }
        }
        return ret;
      }

      /// Invert the matrix.
      Matrix & invert();

      /// Get the inverted matrix.
      inline Matrix inverted() const
      {
        Matrix ret = *this;
        return ret.invert();
      }

      /// Find whether the matrix has normal columns.
      inline bool normalColumns() const
      {
        for (int i = 0; i < N; ++i)
          if (!col[i].normal())
            return false;
        return true;
      }

      /// Find whether the matrix is orthonormal.
      inline bool orthonormal() const
      {
        if (!normalColumns())
          return false;
        for (int i = 0; i < N-1; ++i) {
          for (int j = i+1; j < N; ++j) {
            // FIXME won't work with integer matrices
            float dot = col[i]*col[j];
            if (dot > 0.01f || dot < -0.01f)
              return false;
          }
        }
        return true;
      }

      inline operator std::string() const;
  }; // ::maths::Matrix

  template <typename T>
  class MatrixDummy
  {
    public:
      template <int NN>
      inline friend Matrix<NN,T> scaleMatrix(T v)
      {
        return Matrix<NN,T>::scale(v);
      }
      template <int NN>
      inline friend Matrix<NN,T> rotationMatrix(T v)
      {
        return Matrix<NN,T>::rotation(v);
      }
  };

  template <int N, typename T>
  inline maths::Matrix<N,T> outer(const maths::Vector<N,T> & vertical, const maths::Vector<N,T> & horizontal)
  {
    maths::Matrix<N,T> result;
    for (int i = 0; i < N; ++i) {
      result[i] = horizontal[i] * vertical;
    }
    return result;
  }

  // Quick creation of transformation matrices
  template <int N, typename T>
  inline const maths::Matrix<N,T>& identityMatrix()
  {
    return maths::Matrix<N,T>::identity();
  }
  template <int N, typename T>
  inline maths::Matrix<N,T> scaleMatrix(const maths::Vector<N,T> &v)
  {
    return maths::Matrix<N,T>::scale(v);
  }
  template <int N, typename T, int NN>
  inline maths::Matrix<N,T> scaleMatrix(const maths::Vector<NN,T> &v)
  {
    return maths::Matrix<N,T>::scale(v);
  }
  template <int N, typename T>
  inline maths::Matrix<N,T> translationMatrix(const maths::Vector<N,T> &v)
  {
    return maths::Matrix<N,T>::translation(v);
  }
  template <int N, typename T, int NN>
  inline maths::Matrix<N,T> translationMatrix(const maths::Vector<NN,T> &v)
  {
    return maths::Matrix<N,T>::translation(v);
  }
  template <typename T>
  inline Matrix<2,T> rotationMatrix(T v)
  {
    return Matrix<2,T>::rotation(v);
  }
  template <int N, typename T>
  inline Matrix<N,T> rotationMatrix(T v)
  {
    return Matrix<N,T>::rotation(v);
  }
  template <int N, typename T, typename F>
  inline maths::Matrix<N,T> rotationMatrix(const maths::Vector<N,T> &axis, F theta)
  {
    return maths::Matrix<N,T>::rotation(axis, (T)theta);
  }
  template <int N, typename T, typename F, int NN>
  inline maths::Matrix<N,T> rotationMatrix(const maths::Vector<NN,T> &axis, F theta)
  {
    return maths::Matrix<N,T>::rotation(axis, (T)theta);
  }

  /// Write a matrix to an output stream.
  template <int N, typename T>
  inline std::ostream & operator << (std::ostream & out, const Matrix<N,T> & m)
  {
    out << "[" << m[0];
    for (int i = 1; i < N; ++i) {
      out << ", " << m[i];
    }
    return out << "]";
  }
  /// Convert to a string using the above stream operator
  template <int N, typename T>
  inline Matrix<N,T>::operator std::string() const
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  // From GLM
  inline Matrix<4,float> LookAtMatrix44RH(Vector<3, float> const& eye,
                                          Vector<3, float> const& center,
                                          Vector<3, float> const& up)
  {
    Vector<3, float> const f((center - eye).normalized());
    Vector<3, float> const s(cross(f, up).normalized());
    Vector<3, float> const u(cross(s, f));

    Matrix<4, float> Result((float)1);
    Result[0][0] = s[0];
    Result[1][0] = s[1];
    Result[2][0] = s[2];
    Result[0][1] = u[0];
    Result[1][1] = u[1];
    Result[2][1] = u[2];
    Result[0][2] =-f[0];
    Result[1][2] =-f[1];
    Result[2][2] =-f[2];
    Result[3][0] =-(s * eye);
    Result[3][1] =-(u * eye);
    Result[3][2] = (f * eye);
    return Result;
  }

  // From GLM
  inline Matrix<4, float> LookAtMatrix44LH(Vector<3, float> const& eye,
                                           Vector<3, float> const& center,
                                           Vector<3, float> const& up)
  {
    Vector<3, float> const f((center - eye).normalized());
    Vector<3, float> const s(cross(up, f).normalized());
    Vector<3, float> const u(cross(f, s));

    Matrix<4, float> Result((float)1);
    Result[0][0] = s[0];
    Result[1][0] = s[1];
    Result[2][0] = s[2];
    Result[0][1] = u[0];
    Result[1][1] = u[1];
    Result[2][1] = u[2];
    Result[0][2] = f[0];
    Result[1][2] = f[1];
    Result[2][2] = f[2];
    Result[3][0] = -(s * eye);
    Result[3][1] = -(u * eye);
    Result[3][2] = -(f * eye);
    return Result;
  }

  // From GLM
  inline Matrix<4, float> PerspectiveMatrix44LH_ZO(float fovy, float aspect,
                                                   float zNear, float zFar)
  {
    RUNTIME_ASSERT(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0), "oh dear");

    float const tanHalfFovy = tan(fovy / static_cast<float>(2));

    Matrix<4, float> Result((float)0);
    Result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    Result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    Result[2][2] = zFar / (zFar - zNear);
    Result[2][3] = static_cast<float>(1);
    Result[3][2] = -(zFar * zNear) / (zFar - zNear);
    return Result;
  }
} // ::maths

#endif // _MATHS_MATRIX_H_
