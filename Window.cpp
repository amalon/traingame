#include "Window.h"

Window::Window(const std::string &newTitle, Renderer *newRenderer)
: title(newTitle),
  renderer(newRenderer),
  redraw(false)
{
}

void Window::mouseWheel(int dx, int dy, bool flipped)
{
    renderer->getViewpoint().zoom(-0.1 * dy);
    redraw = true;
}
