#ifndef TRAINS_WINDOW_SDL_H
#define TRAINS_WINDOW_SDL_H

#include "Window.h"

#include <SDL.h>

class WindowSDL : public Window
{
private:
    SDL_Window *window;
    bool inited;
public:
    WindowSDL(const std::string &newTitle, Renderer *newRenderer);
    virtual ~WindowSDL();

    virtual bool init();

    virtual bool handleEvents(unsigned int waitMs);

    virtual void renderFrame();
};

#endif // TRAINS_WINDOW_SDL_H
