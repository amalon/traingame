#include <iostream>

#include "GaugeDataStore.h"
#include "RendererOpenGL.h"
#include "WindowSDL.h"

int main(int argc, char **argv)
{
    const Gauge *standard = gaugeDataStore.get("standard");
    std::cout << *standard << std::endl;

    RendererOpenGL renderer;
    WindowSDL window("Tracks", &renderer);
    if (!window.init()) {
        return 1;
    }

    const unsigned int delayMs = 1000;
    while (window.handleEvents(delayMs)) {
        window.renderFrame();
    };

    return 0;
}
