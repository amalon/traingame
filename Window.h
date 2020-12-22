#ifndef TRAINS_WINDOW_H
#define TRAINS_WINDOW_H

#include "Renderer.h"
#include "Vector.h"

#include <string>

class Window
{
protected:
    std::string title;
    Renderer *renderer;
    int width, height;
    Vec2f mouseViewport;
public:
    Window(const std::string &newTitle, Renderer *newRenderer);
    virtual ~Window()
    {
    }

    Vec2f screenToViewport(Vec2f screen);

    virtual bool init() = 0;

    /// Handle input events and return whether the window is still open
    virtual bool handleEvents(unsigned int waitMs) = 0;

    virtual void renderFrame() = 0;

protected:
    // Generic notifications
    void mouseMove(int x, int y, int dx, int dy);
    void mouseLeave();
    void mouseDown(int x, int y, int button, int clicks);
    void mouseUp(int x, int y, int button, int clicks);
    void mouseWheel(int dx, int dy, bool flipped);
};

#endif // TRAINS_WINDOW_H
