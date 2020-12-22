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
    } else {
        // mouseRay doesn't change when panning
        mouseRay = ray;
    }
    renderer->setRedraw();
}

void NavigateMode::mouseLeave()
{
    mouseRay.norm.set(0);
    renderer->setRedraw();
}

void NavigateMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Right click pans
    if (button == 1)
        panning = true;
    // Colour changed
    renderer->setRedraw();
}

void NavigateMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    if (button == 1)
        panning = false;
    // Colour changed
    renderer->setRedraw();
}

void NavigateMode::mouseWheel(int dx, int dy, bool flipped)
{
    renderer->getViewpoint().zoom(-0.1 * dy);
    bool oldPanning = panning;
    panning = true;
    renderer->viewportChanged();
    panning = oldPanning;
}

void NavigateMode::renderUI()
{
    // FIXME OpenGL specific...
    if (!mouseRay.norm.zero()) {
        glPointSize(15);
        glBegin(GL_POINTS);
        if (panning)
            glColor3f(1, 0, 0);
        else
            glColor3f(0, 1, 0);
        glVertex3f(mouseRay.start[0],
                   mouseRay.start[1],
                   mouseRay.start[2]);
        glEnd();
    }
}
