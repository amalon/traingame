#include "Window.h"

Window::Window(const std::string &newTitle, Renderer *newRenderer)
: title(newTitle),
  renderer(newRenderer),
  redraw(false)
{
}
