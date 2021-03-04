#ifndef TRAINS_CONTROL_MODE_H
#define TRAINS_CONTROL_MODE_H

#include "Renderable.h"
#include "Vector.h"

class Renderer;

class ControlMode : public Renderable
{
protected:
    Renderer *renderer;

public:
    ControlMode();
    virtual ~ControlMode()
    {
    }

    void setRenderer(Renderer *newRenderer)
    {
        renderer = newRenderer;
    }

    virtual void activate();
    virtual void deactivate();

    virtual void mouseMove(const LineUnit3f &ray);
    virtual void mouseLeave();
    virtual void mouseDown(const LineUnit3f &ray, int button, int clicks);
    virtual void mouseUp(const LineUnit3f &ray, int button, int clicks);
    virtual void mouseWheel(int dx, int dy, bool flipped);

    virtual void viewportChanged();
};

#endif // TRAINS_CONTROL_MODE_H
