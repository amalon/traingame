#ifndef TRAINS_RENDERER_H
#define TRAINS_RENDERER_H

#include "Renderable.h"
#include "Viewpoint.h"
#include "Vector.h"

class ControlMode;
class Railway;

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
    Railway *railway;

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

    void viewportChanged();

    Viewpoint &getViewpoint()
    {
        return viewpoint;
    }
    const Viewpoint &getViewpoint() const
    {
        return viewpoint;
    }

    void setActiveMode(ControlMode *mode);
    void setNavigateMode(ControlMode *mode);

    void setRailway(Railway *newRailway);

    void setRedraw()
    {
        redraw = true;
    }

    bool needsRedraw() const
    {
        return redraw;
    }

    void renderFrame();

    virtual void renderObj(Renderable *obj) = 0;
};

#endif // TRAINS_RENDERER_H
