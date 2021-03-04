#ifndef TRAINS_VECTOR_H
#define TRAINS_VECTOR_H

#include <maths/Vector.h>
#include <maths/Matrix.h>
#include <maths/Line.h>

typedef maths::Vector<2, float> Vec2f;
typedef maths::Vector<3, float> Vec3f;

typedef maths::Matrix<2, float> Mat22f;

typedef maths::Line<3, float, maths::LineUnit> LineUnit3f;

#endif // TRAINS_VECTOR_H
