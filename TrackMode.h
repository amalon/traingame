#ifndef TRAINS_TRACK_MODE_H
#define TRAINS_TRACK_MODE_H

#include "ControlMode.h"
#include "Vector.h"

#include <vector>

class TrackNode;
class TrackSection;
class TrackSpec;
class Railway;

class TrackMode : public ControlMode
{
private:
    Railway *railway;
    const TrackSpec *minSpec;

    TrackNode *hoverNode;
    TrackNode *selectedNode;

    bool testTrain;

    enum DragMode {
        NONE,
        MOVE,
        ROTATE,
        RECURVE,
        POINTS
    } dragMode;
    int dragIndex;
    LineUnit3f mouseRay;
    bool mouseMoved;

    struct Handle {
        bool enabled;
        Vec3f position;
        enum DragMode mode;
        int index;
    };
    std::vector<struct Handle> handles;

public:
    TrackMode(Railway *newRailway, const TrackSpec *newMinSpec);

    virtual void mouseMove(const LineUnit3f &ray);
    virtual void mouseLeave();
    virtual void mouseDown(const LineUnit3f &ray, int button, int clicks);
    virtual void mouseUp(const LineUnit3f &ray, int button, int clicks);

    virtual void viewportChanged();

    RENDERABLE_GL();

private:
    void updateHandles();
    void renderHandlesGL(RendererOpenGL * renderer);
    const Handle *handleUnderMouse(const LineUnit3f &ray, float size) const;
};

#endif // TRAINS_TRACK_MODE_H
