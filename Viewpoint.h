#ifndef TRAINS_VIEWPOINT_H
#define TRAINS_VIEWPOINT_H

#include "Vector.h"

class Viewpoint
{
private:
    /// Target coordinates
    Vec3f target;
    /// Distance
    float distance;

    // Aspect ratio (width / height)
    float aspect;

public:
    Viewpoint();

    void setAspect(float newAspect)
    {
        aspect = newAspect;
    }

    void pan(Vec3f delta);
    void zoom(float ddist);

    const Vec3f &getTarget() const
    {
        return target;
    }

    float getDistance() const
    {
        return distance;
    }

    // Orthographic boundaries
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    float getNear() const;
    float getFar() const;

    LineUnit3f viewportToWorldRay(const Vec2f &viewport) const;
    float viewportToWorldSize(const Vec3f &pos, float size) const;
};

#endif // TRAINS_VIEWPOINT_H
