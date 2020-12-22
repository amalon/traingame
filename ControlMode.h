#ifndef TRAINS_CONTROL_MODE_H
#define TRAINS_CONTROL_MODE_H

#include <Vector.h>

class Renderer;

class ControlMode
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

    virtual void mouseMove(const LineNormal3f &ray);
    virtual void mouseLeave();
    virtual void mouseDown(const LineNormal3f &ray, int button, int clicks);
    virtual void mouseUp(const LineNormal3f &ray, int button, int clicks);
    virtual void mouseWheel(int dx, int dy, bool flipped);

    virtual void renderUI();
};

#endif // TRAINS_CONTROL_MODE_H
