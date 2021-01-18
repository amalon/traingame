#include "GaugeDataStore.h"
#include "TrackSpec.h"
#include "Railway.h"
#include "RendererOpenGL.h"
#include "TrackMode.h"
#include "WindowSDL.h"

#include <iostream>
#include <SDL.h>

int main(int argc, char **argv)
{
    const Gauge *standard = gaugeDataStore.get("standard");
    std::cout << *standard << std::endl;
    TrackSpec *standardSpec = new TrackSpec();
    standardSpec->setName("standard");
    standardSpec->setTrackGauge(standard);
    standardSpec->setTrackSpacing(3.0f);
    standardSpec->setMaxCurvature(1.0f / 20.0f);
    standardSpec->setMinCurvature(1.0f / 30.0f);
    standardSpec->setMaxCurvatureRate(standardSpec->getMaxCurvature() / 10.0f);
    standardSpec->setMinCurvatureRate(standardSpec->getMinCurvature() / 10.0f);

    Railway *railway = new Railway();

    RendererOpenGL renderer;
    TrackMode *trackMode = new TrackMode(railway, standardSpec);
    renderer.setActiveMode(trackMode);
    renderer.setRailway(railway);

    WindowSDL window("Tracks", &renderer);
    if (!window.init()) {
        return 1;
    }

    unsigned int targetFps = 30;
    unsigned int minFps = 15;
    int delayMs = 0;
    unsigned int ticks = SDL_GetTicks();

    constexpr bool showFps = false;
    int frame = 0;
    unsigned int lastFpsTicks = ticks;

    while (window.handleEvents(delayMs)) {
        unsigned int nextTicks = SDL_GetTicks();
        unsigned int deltaTicks = nextTicks - ticks;
        ticks = nextTicks;
        float dt = (float)deltaTicks / 1000;
        if (dt > 1.0f/minFps)
            dt = 1.0f/minFps;

        railway->advance(dt);
        renderer.setRedraw();
        window.renderFrame();

        unsigned int finalTicks = SDL_GetTicks();
        delayMs = nextTicks + (unsigned int)(1000.0f/targetFps) - finalTicks;
        if (delayMs < 0)
            delayMs = 0;

        if (showFps && ++frame > 60*10) {
            float sec = (float)(finalTicks - lastFpsTicks) / 1000;
            std::cout << frame/sec << " fps" << std::endl;
            frame = 0;
            lastFpsTicks = finalTicks;
        }
    };

    return 0;
}
