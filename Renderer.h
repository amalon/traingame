#ifndef TRAINS_RENDERER_H
#define TRAINS_RENDERER_H

#include "Viewpoint.h"

// Renderer of map
class Renderer
{
protected:
    Viewpoint viewpoint;

public:
    virtual void setViewport(int width, int height) = 0;

    Viewpoint &getViewpoint()
    {
        return viewpoint;
    }

    virtual void renderFrame() = 0;
};

#endif // TRAINS_RENDERER_H
