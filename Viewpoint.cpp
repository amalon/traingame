#include "Viewpoint.h"

#include <cmath>

Viewpoint::Viewpoint()
: target(0.0f),
  distance(100),
  aspect(1.0f)
{
}

void Viewpoint::pan(Vec3f delta)
{
    target += delta;
}

void Viewpoint::zoom(float ddist)
{
    distance *= std::exp(ddist);
    if (distance < 1)
        distance = 1;
}

// Orthographic boundaries
float Viewpoint::getLeft() const
{
    return -distance * aspect;
}
float Viewpoint::getRight() const
{
    return distance * aspect;
}
float Viewpoint::getTop() const
{
    return distance;
}
float Viewpoint::getBottom() const
{
    return -distance;
}
float Viewpoint::getNear() const
{
    return 100;
}
float Viewpoint::getFar() const
{
    return -100;
}

LineNormal3f Viewpoint::viewportToWorldRay(const Vec2f &viewport) const
{
    LineNormal3f ret;

    ret.start[0] = target[0] + getLeft() + viewport[0] * (getRight() - getLeft());
    ret.start[1] = target[1] + getTop() + viewport[1] * (getBottom() - getTop());
    ret.start[2] = target[2] + distance;

    ret.norm[0] = 0.0f;
    ret.norm[1] = 0.0f;
    ret.norm[2] = -1.0f;

    return ret;
}

float Viewpoint::viewportToWorldSize(const Vec3f &pos, float size) const
{
    return size * (distance * 2);
}
