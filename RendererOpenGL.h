#ifndef TRAINS_RENDERER_OPENGL_H
#define TRAINS_RENDERER_OPENGL_H

#include "Renderer.h"

// Renderer of map
class RendererOpenGL : public Renderer
{
public:
    RendererOpenGL();

protected:
    virtual void vRenderFrame();
    virtual void vSetViewport(int width, int height);
    virtual void renderObj(Renderable *obj);

private:
    void setupViewpoint();
};

#endif // TRAINS_RENDERER_OPENGL_H
