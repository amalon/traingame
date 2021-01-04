#include "Renderer.h"
#include "NavigateMode.h"

Renderer::Renderer()
: activeMode(nullptr),
  navigateMode(nullptr),
  redraw(false),
  railway(nullptr)
{
    mouseViewport.set(0);
    mouseRay.start.set(0);
    mouseRay.norm.set(0);

    setNavigateMode(new NavigateMode());
}

Renderer::~Renderer()
{
    setActiveMode(nullptr);
    setNavigateMode(nullptr);
}

void Renderer::setViewport(int width, int height)
{
    if (height)
        viewpoint.setAspect((float)width / height);
    vSetViewport(width, height);
}

void Renderer::mouseMove(Vec2f viewport)
{
    mouseViewport = viewport;
    mouseRay = viewpoint.viewportToWorldRay(mouseViewport);
    if (activeMode)
        activeMode->mouseMove(mouseRay);
    if (navigateMode)
        navigateMode->mouseMove(mouseRay);
}

void Renderer::mouseLeave()
{
    mouseRay.norm.set(0);
    if (activeMode)
        activeMode->mouseLeave();
    if (navigateMode)
        navigateMode->mouseLeave();
}

void Renderer::mouseDown(Vec2f viewport, int button, int clicks)
{
    if (activeMode)
        activeMode->mouseDown(mouseRay, button, clicks);
    if (navigateMode)
        navigateMode->mouseDown(mouseRay, button, clicks);
}

void Renderer::mouseUp(Vec2f viewport, int button, int clicks)
{
    if (activeMode)
        activeMode->mouseUp(mouseRay, button, clicks);
    if (navigateMode)
        navigateMode->mouseUp(mouseRay, button, clicks);
}

void Renderer::mouseWheel(int dx, int dy, bool flipped)
{
    if (activeMode)
        activeMode->mouseWheel(dx, dy, flipped);
    if (navigateMode)
        navigateMode->mouseWheel(dx, dy, flipped);
}

void Renderer::viewportChanged()
{
    mouseMove(mouseViewport);
    if (activeMode)
        activeMode->viewportChanged();
    if (navigateMode)
        navigateMode->viewportChanged();
}

void Renderer::setActiveMode(ControlMode *mode)
{
    if (activeMode) {
        activeMode->deactivate();
        delete activeMode;
        activeMode = nullptr;
    }
    if (mode) {
        activeMode = mode;
        activeMode->setRenderer(this);
        activeMode->activate();
    }
}

void Renderer::setNavigateMode(ControlMode *mode)
{
    if (navigateMode) {
        navigateMode->deactivate();
        delete navigateMode;
        navigateMode = nullptr;
    }
    if (mode) {
        navigateMode = mode;
        navigateMode->setRenderer(this);
        navigateMode->activate();
    }
}

void Renderer::setRailway(Railway *newRailway)
{
    railway = newRailway;
}

void Renderer::renderFrame()
{
    vRenderFrame();
    redraw = false;
}
