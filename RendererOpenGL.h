#ifndef TRAINS_RENDERER_OPENGL_H
#define TRAINS_RENDERER_OPENGL_H

#include "Renderer.h"

// Renderer of map
class RendererOpenGL : public Renderer
{
    float aspect;

public:
    RendererOpenGL();

    virtual void setViewport(int width, int height);

    virtual void renderFrame();

private:
    void setupViewpoint();
};

#endif // TRAINS_RENDERER_OPENGL_H
