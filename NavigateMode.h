#ifndef TRAINS_NAVIGATE_MODE_H
#define TRAINS_NAVIGATE_MODE_H

#include "ControlMode.h"

class NavigateMode : public ControlMode
{
private:
    LineUnit3f mouseRay;
    bool panning;

public:
    NavigateMode();

    virtual void mouseMove(const LineUnit3f &ray);
    virtual void mouseLeave();
    virtual void mouseDown(const LineUnit3f &ray, int button, int clicks);
    virtual void mouseUp(const LineUnit3f &ray, int button, int clicks);
    virtual void mouseWheel(int dx, int dy, bool flipped);
};

#endif // TRAINS_NAVIGATE_MODE_H
