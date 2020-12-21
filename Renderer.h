#ifndef TRAINS_RENDERER_H
#define TRAINS_RENDERER_H

// Renderer of map
class Renderer
{
public:

    virtual void setViewport(int width, int height) = 0;
    virtual void renderFrame() = 0;
};

#endif // TRAINS_RENDERER_H
