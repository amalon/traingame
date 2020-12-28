#ifndef TRAINS_RENDERABLE_H
#define TRAINS_RENDERABLE_H

class Renderer;
class RendererOpenGL;

#define RENDERABLE_GENERIC() \
    virtual void render(Renderer *renderer)

#define RENDERABLE_GL() \
    virtual void renderGL(RendererOpenGL *renderer)

class Renderable
{
public:
    RENDERABLE_GENERIC();
    RENDERABLE_GL();
};

#endif // TRAINS_RENDERABLE_H
