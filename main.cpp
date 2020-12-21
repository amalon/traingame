#include <iostream>

#include "GaugeDataStore.h"

int main(int argc, char **argv)
{
    const Gauge *standard = gaugeDataStore.get("standard");
    std::cout << *standard << std::endl;

    return 0;
}
