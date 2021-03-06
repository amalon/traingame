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
    typedef maths::Matrix<2, Length> Mat22l;


private:
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
    // Total length of curve
    Length length;

public:
    Clothoid()
    : startPosition((L)0),
      startDirection(0),
      startCurvature(0),
      curvatureRate(0),
      curvatureRateSqrt(0),
      length(0)
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
    void setLength(Length newLength)
    {
        length = newLength;
    }

    // Accessors

    Vec2l getStartPosition() const
    {
        return startPosition;
    }
    Vec2l getStartParallelPosition(float leftOffset) const
    {
        Vec2l leftVec;
        maths::sincos((Length)(startDirection + M_PI/2), &leftVec[1], &leftVec[0]);
        return startPosition + leftVec * leftOffset;
    }
    Vec2l getStartParallelPosition(float leftOffset, Mat22l *outRotMatrix) const
    {
        Vec2l leftVec;
        maths::sincos((Length)(startDirection + M_PI/2), &leftVec[1], &leftVec[0]);
        if (outRotMatrix) {
            (*outRotMatrix)[1] = leftVec;
            (*outRotMatrix)[0][0] = leftVec[1];
            (*outRotMatrix)[0][1] = -leftVec[0];
        }
        return startPosition + leftVec * leftOffset;
    }
    Angle getStartDirection() const
    {
        return startDirection;
    }
    Curvature getStartCurvature() const
    {
        return startCurvature;
    }
    CurvatureRate getCurvatureRate() const
    {
        return curvatureRate;
    }
    Length getLength() const
    {
        return length;
    }
    Length getParallelLength(Length leftOffset) const
    {
        if (!leftOffset)
            return length;
        if (curvatureRate != 0) {
            float avgCurvature = (startCurvature + getEndCurvature()) / 2;
            // FIXME calculate correctly
            return length * ((Length)1 - leftOffset * avgCurvature);
        } else if (startCurvature != 0) {
            return length * ((Length)1 - leftOffset * startCurvature);
        } else {
            return length;
        }
    }

    // Find information about the end
    Vec2l getEndPosition() const
    {
        return positionAtLength(length);
    }
    Angle getEndDirection() const
    {
        return directionAtLength(length);
    }
    Curvature getEndCurvature() const
    {
        return curvatureAtLength(length);
    }
    Angle getEndDirectionChange() const
    {
        return directionChangeAtLength(length);
    }

    // Get a clothoid that follows on from this one
    Clothoid getNextClothoid() const
    {
        Clothoid ret;
        ret.setStartPosition(getEndPosition());
        ret.setStartDirection(getEndDirection());
        ret.setStartCurvature(getEndCurvature());
        return ret;
    }

    // Calculators

    // Find the curvature at distance length
    Curvature curvatureAtLength(Length length) const
    {
        return startCurvature + curvatureRate * length;
    }
    // Find the curvature of a parallel line at distance length
    Curvature parallelCurvatureAtLength(Length leftOffset, Length length) const
    {
        return (Curvature)1 / ((Curvature)1 / curvatureAtLength(length) - leftOffset);
    }

    // Find center of turning circle at start
    Vec2l circleAtStart() const
    {
        Vec2l directionVec;
        maths::sincos((Length)(startDirection + M_PI/2), &directionVec[1], &directionVec[0]);
        return startPosition + directionVec / startCurvature;
    }
    // Find center of turning circle at distance length
    Vec2l circleAtLength(Length length) const
    {
        float curvature = curvatureAtLength(length);
        float direction = directionAtLength(length);
        Vec2l directionVec;
        maths::sincos(direction + M_PI/2, &directionVec[1], &directionVec[0]);
        return positionAtLength(length) + directionVec / curvature;
    }

    // Find the direction at distance length
    Angle directionChangeAtLength(Length length) const
    {
        return length * (startCurvature + curvatureRate * length / 2);
    }

    // Find the direction at distance length
    Angle directionAtLength(Length length) const
    {
        return startDirection + directionChangeAtLength(length);
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
        Mat22l matrix;
        maths::sincos((Length)rotation, &matrix[0][1], &matrix[0][0]);
        matrix[1][0] = -matrix[0][1];
        matrix[1][1] = matrix[0][0];
        return startPosition + matrix * fres;
    }

    Vec2f parallelPositionAtLength(Length leftOffset, Length length) const
    {
        Vec2f position = positionAtLength(length);
        Angle direction = directionAtLength(length);
        Vec2l leftVec;
        maths::sincos((Length)(direction + M_PI/2), &leftVec[1], &leftVec[0]);
        return position + leftVec * leftOffset;
    }

    Vec2f parallelPositionAtParallelLength(Length leftOffset, Length parallelDistance,
                                           Mat22l *outRotMatrix = nullptr) const
    {
        // FIXME this isn't quite accurate for variable curvature
        float totalParallelLength = getParallelLength(leftOffset);
        float midLength = length * parallelDistance / totalParallelLength;
        Vec2f position = positionAtLength(midLength);
        Angle direction = directionAtLength(midLength);
        Vec2l leftVec;
        maths::sincos((Length)(direction + M_PI/2), &leftVec[1], &leftVec[0]);
        if (outRotMatrix) {
            (*outRotMatrix)[1] = leftVec;
            (*outRotMatrix)[0][0] = leftVec[1];
            (*outRotMatrix)[0][1] = -leftVec[0];
        }
        return position + leftVec * leftOffset;
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
