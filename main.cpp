#include <iostream>

#include "GaugeDataStore.h"
#include "TrackSpec.h"
#include "Railway.h"
#include "RendererOpenGL.h"
#include "TrackMode.h"
#include "WindowSDL.h"

int main(int argc, char **argv)
{
    const Gauge *standard = gaugeDataStore.get("standard");
    std::cout << *standard << std::endl;
    TrackSpec *standardSpec = new TrackSpec();
    standardSpec->setName("standard");
    standardSpec->setTrackGauge(standard);
    standardSpec->setTrackSpacing(3.0f);

    Railway *railway = new Railway();

    RendererOpenGL renderer;
    TrackMode *trackMode = new TrackMode(railway, standardSpec);
    renderer.setActiveMode(trackMode);
    renderer.setRailway(railway);

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
