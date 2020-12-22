#include "WindowSDL.h"

WindowSDL::WindowSDL(const std::string &newTitle, Renderer *newRenderer)
: Window(newTitle, newRenderer),
  inited(false)
{
}

WindowSDL::~WindowSDL()
{
    if (inited)
        SDL_Quit();
}

bool WindowSDL::init()
{
    if (inited)
        return true;

    if (SDL_Init(SDL_INIT_TIMER) < 0)
        return false;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              800, 600,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Quit();
        return false;
    }

    SDL_GetWindowSize(window, &width, &height);
    SDL_GL_CreateContext(window);

    renderer->setViewport(width, height);

    inited = true;
    return true;
}

// After some timeout, push an event to wake up SDL_WaitEvent
static Uint32 timerCallback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userEvent;

    userEvent.type = SDL_USEREVENT;
    userEvent.code = 0;
    userEvent.data1 = nullptr;
    userEvent.data2 = nullptr;

    event.type = SDL_USEREVENT;
    event.user = userEvent;

    SDL_PushEvent(&event);
    return 0;
}

bool WindowSDL::handleEvents(unsigned int waitMs)
{
    if (!inited)
        return false;

    // If no events are pending, arrange for a timeout so we don't wait
    // indefinitely
    if (waitMs && !SDL_PollEvent(nullptr)) {
        SDL_TimerID timer = SDL_AddTimer(waitMs, timerCallback, nullptr);
        SDL_WaitEvent(nullptr);
        SDL_RemoveTimer(timer);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_EXPOSED:
                renderer->setRedraw();
                break;
            case SDL_WINDOWEVENT_RESIZED:
                width = event.window.data1;
                height = event.window.data2;
                renderer->setViewport(width, height);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                mouseLeave();
                break;
            };
            break;

        case SDL_MOUSEMOTION:
            mouseMove(event.motion.x, event.motion.y,
                      event.motion.xrel, event.motion.yrel);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            int button = -1;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    button = 0;
                    break;
                case SDL_BUTTON_MIDDLE:
                    button = 2;
                    break;
                case SDL_BUTTON_RIGHT:
                    button = 1;
                    break;
            }
            if (button < 0)
                break;
            if (event.type == SDL_MOUSEBUTTONDOWN)
                mouseDown(event.button.x, event.button.y,
                          button, event.button.clicks);
            else
                mouseUp(event.button.x, event.button.y,
                        button, event.button.clicks);
            break;
        }

        case SDL_MOUSEWHEEL:
            mouseWheel(event.wheel.x, event.wheel.y,
                       event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED);
            break;

        case SDL_QUIT:
            return false;
        }
    }
    return true;
}

void WindowSDL::renderFrame()
{
    if (!inited)
        return;

    if (renderer->needsRedraw()) {
        renderer->renderFrame();
        SDL_GL_SwapWindow(window);
    }
}
