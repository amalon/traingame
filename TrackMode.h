#ifndef TRAINS_TRACK_MODE_H
#define TRAINS_TRACK_MODE_H

#include "ControlMode.h"
#include "Railway.h"
#include "Vector.h"

class TrackMode : public ControlMode
{
private:
    Railway *railway;
    int step;
    Vec3f start;
    Vec3f end;

    TrackNode *hoverNode;
    TrackNode *selectedNode;

    enum {
        NONE,
        MOVE,
        ROTATE,
        RECURVE
    } dragMode;
    LineNormal3f mouseRay;

public:
    TrackMode(Railway *newRailway);

    virtual void mouseMove(const LineNormal3f &ray);
    virtual void mouseLeave();
    virtual void mouseDown(const LineNormal3f &ray, int button, int clicks);
    virtual void mouseUp(const LineNormal3f &ray, int button, int clicks);

    virtual void renderUI();
};

#endif // TRAINS_TRACK_MODE_H
