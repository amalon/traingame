#ifndef TRAINS_RENDERER_OPENGL_H
#define TRAINS_RENDERER_OPENGL_H

#include "Renderer.h"

// Renderer of map
class RendererOpenGL : public Renderer
{
public:
    RendererOpenGL();

    virtual void renderObj(Renderable *obj);

protected:
    virtual void vRenderFrame();
    virtual void vSetViewport(int width, int height);

private:
    void setupViewpoint();
};

#endif // TRAINS_RENDERER_OPENGL_H
