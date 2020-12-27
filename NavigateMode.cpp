#include "NavigateMode.h"
#include "Renderer.h"

#include <GL/gl.h>

NavigateMode::NavigateMode()
: panning(false)
{
    mouseRay.start.set(0);
    mouseRay.norm.set(0);
}

void NavigateMode::mouseMove(const LineNormal3f &ray)
{
    if (panning && !mouseRay.norm.zero()) {
        Vec3f diff = ray.start - mouseRay.start;
        renderer->getViewpoint().pan(-diff);
        renderer->setRedraw();
    } else {
        // mouseRay doesn't change when panning
        mouseRay = ray;
    }
}

void NavigateMode::mouseLeave()
{
    mouseRay.norm.set(0);
}

void NavigateMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Right click pans
    if (button == 1)
        panning = true;
}

void NavigateMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    if (button == 1)
        panning = false;
}

void NavigateMode::mouseWheel(int dx, int dy, bool flipped)
{
    renderer->getViewpoint().zoom(-0.1 * dy);
    bool oldPanning = panning;
    panning = true;
    renderer->viewportChanged();
    panning = oldPanning;
}
