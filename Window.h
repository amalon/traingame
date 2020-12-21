#ifndef TRAINS_WINDOW_H
#define TRAINS_WINDOW_H

#include "Renderer.h"

#include <string>

class Window
{
protected:
    std::string title;
    Renderer *renderer;
    int width, height;
    bool redraw;
public:
    Window(const std::string &newTitle, Renderer *newRenderer);
    virtual ~Window()
    {
    }

    virtual bool init() = 0;

    /// Handle input events and return whether the window is still open
    virtual bool handleEvents(unsigned int waitMs) = 0;

    virtual void renderFrame() = 0;
};

#endif // TRAINS_WINDOW_H
