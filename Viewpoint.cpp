#include "Viewpoint.h"

#include <cmath>

Viewpoint::Viewpoint()
: target(),
  distance(100)
{
}

void Viewpoint::pan(float dx, float dy)
{
    target[0] += dx;
    target[1] += dy;
}

void Viewpoint::zoom(float ddist)
{
    distance *= std::exp(ddist);
    if (distance < 1)
        distance = 1;
}
