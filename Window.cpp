#include "Window.h"

Window::Window(const std::string &newTitle, Renderer *newRenderer)
: title(newTitle),
  renderer(newRenderer),
  mouseViewport(0.0f)
{
}

Vec2f Window::screenToViewport(Vec2f screen)
{
    return Vec2f(screen[0] / width,
                 screen[1] / height);
}

void Window::mouseMove(int x, int y, int dx, int dy)
{
    mouseViewport = screenToViewport(Vec2f(x, y));
    renderer->mouseMove(mouseViewport);
}

void Window::mouseLeave()
{
    renderer->mouseLeave();
}

void Window::mouseDown(int x, int y, int button, int clicks)
{
    mouseViewport = screenToViewport(Vec2f(x, y));
    renderer->mouseDown(mouseViewport, button, clicks);
}

void Window::mouseUp(int x, int y, int button, int clicks)
{
    mouseViewport = screenToViewport(Vec2f(x, y));
    renderer->mouseUp(mouseViewport, button, clicks);
}

void Window::mouseWheel(int dx, int dy, bool flipped)
{
    renderer->mouseWheel(dx, dy, flipped);
}
