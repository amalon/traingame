#ifndef TRAINS_CLOTHOID_H
#define TRAINS_CLOTHOID_H

#include <maths/Vector.h>
#include <maths/Matrix.h>

#include <Fresnel.h>

#include <cmath>

template <typename L, typename C>
class Clothoid
{
public:
    typedef L Length;
    typedef C Angle;
    typedef Angle Curvature;
    typedef Angle CurvatureRate;
    typedef maths::Vector<2, Length> Vec2l;

    // Start position of clothoid
    Vec2l startPosition;
    // Start direction, counterclockwise from east (1, 0) towards north (0, 1)
    Angle startDirection;
    // Starting curvature [radians per length] (inverse of turning circle)
    Curvature startCurvature;
    // Rate of change of curvature [radians per length per length]
    CurvatureRate curvatureRate;
    // Signed square root of absolute curvature rate
    CurvatureRate curvatureRateSqrt;

    Clothoid()
    : startPosition((L)0),
      startDirection(0),
      startCurvature(0),
      curvatureRate(0),
      curvatureRateSqrt(0)
    {
    }

    // Setters
    void setStartPosition(const Vec2l &newStartPosition)
    {
        startPosition = newStartPosition;
    }
    void setStartDirection(Angle newStartDirection)
    {
        startDirection = newStartDirection;
    }
    void setStartCurvature(Curvature newStartCurvature)
    {
        startCurvature = newStartCurvature;
    }
    void setCurvatureRate(CurvatureRate newCurvatureRate)
    {
        curvatureRate = newCurvatureRate;
        if (newCurvatureRate >= 0)
            curvatureRateSqrt = sqrt(newCurvatureRate);
        else
            curvatureRateSqrt = -sqrt(-newCurvatureRate);
    }

    // Accessors

    // Find the curvature at distance length
    Curvature curvatureAtLength(Length length) const
    {
        return startCurvature + curvatureRate * length;
    }

    // Find the direction at distance length
    Angle directionAtLength(Length length) const
    {
        return startDirection + length * (startCurvature + curvatureRate * length / 2);
    }

    // Find the 2D position at distance length
    Vec2l positionAtLength(Length length) const
    {
        Vec2l fres;
        Angle fresRotation;
        if (curvatureRate != 0) {
            constexpr Length invSqrt2 = (Length)1/sqrt(2);
            const Length fresnelScale = curvatureRateSqrt / invSqrt2 / 2;
            // Length from zero curvature
            Length lengthOffs = -lengthAtCurvature(0);
            // 2D fresnel offset from zero curvature
            Vec2l fresOffs = fresnel(lengthOffs * fresnelScale);

            // Calculate 2D fresnel for offset & scaled length
            fres = (fresnel((length + lengthOffs) * fresnelScale) - fresOffs) / fresnelScale;
            fresRotation = fresnelDirection(lengthOffs * fresnelScale);

            // Compensate for negative change in curvature
            if (curvatureRate < 0) {
                fres[1] = -fres[1];
                fresRotation = -fresRotation;
            }
        } else if (startCurvature != 0) {
            // Circle
            Length radius = (Length)1 / startCurvature;
            Length absRadius = (radius < 0 ? -radius : radius);
            Length sin, cos;
            maths::sincos(length / absRadius, &sin, &cos);
            fres[0] = sin * absRadius;
            fres[1] = radius - cos * radius;
            fresRotation = 0;
        } else {
            // Straight line
            fres[0] = length;
            fres[1] = 0;
            fresRotation = 0;
        }

        // Rotate the output to compensate for startDirection and startCurvature
        Angle rotation = startDirection - fresRotation;
        maths::Matrix<2, Length> matrix;
        Length sinRot = sin(rotation);
        Length cosRot = cos(rotation);
        matrix[0][0] = cosRot; matrix[1][0] = -sinRot;
        matrix[0][1] = sinRot; matrix[1][1] = cosRot;
        return startPosition + matrix * fres;
    }

    // Find the distance to curvature finalCurvature
    Length lengthAtCurvature(Curvature finalCurvature) const
    {
        return (finalCurvature - startCurvature) / curvatureRate;
    }

    // Find the direction at curvature finalCurvature
    Angle directionAtCurvature(Curvature finalCurvature) const
    {
        return directionAtLength(lengthAtCurvature(finalCurvature));
    }
};

#endif // TRAINS_CLOTHOID_H
