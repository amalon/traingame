#ifndef TRAINS_RENDERER_H
#define TRAINS_RENDERER_H

#include "ControlMode.h"
#include "Viewpoint.h"
#include "Vector.h"

// Renderer of map
class Renderer
{
protected:
    Viewpoint viewpoint;
    Vec2f mouseViewport;
    LineNormal3f mouseRay;
    ControlMode *activeMode;
    ControlMode *navigateMode;
    bool redraw;

    virtual void vSetViewport(int width, int height) = 0;
    virtual void vRenderFrame() = 0;

public:
    Renderer();
    virtual ~Renderer();

    void setViewport(int width, int height);
    void mouseMove(Vec2f viewport);
    void mouseLeave();
    void mouseDown(Vec2f viewport, int button, int clicks);
    void mouseUp(Vec2f viewport, int button, int clicks);
    void mouseWheel(int dx, int dy, bool flipped);

    void viewportChanged()
    {
        mouseMove(mouseViewport);
    }

    Viewpoint &getViewpoint()
    {
        return viewpoint;
    }

    void setActiveMode(ControlMode *mode);
    void setNavigateMode(ControlMode *mode);

    void setRedraw()
    {
        redraw = true;
    }

    bool needsRedraw() const
    {
        return redraw;
    }

    void renderFrame();
};

#endif // TRAINS_RENDERER_H
