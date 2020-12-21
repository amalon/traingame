#include "GaugeDataStore.h"
#include "RailProfileDataStore.h"

typedef nlohmann::json json;

GaugeDataStore gaugeDataStore("data/gauge/");

template <>
Gauge *GaugeDataStore::load(const json &data)
{
    Gauge *gauge = new Gauge();

    gauge->setName(data["name"]);
    gauge->setGauge(data["gauge"]);
    json rails = data["rails"];
    for (json::iterator it = rails.begin(); it != rails.end(); ++it) {
        json rail = *it;
        const RailProfile *profile = railProfileDataStore.get(rail["profile"]);
        float x, y, dx, dy = 0;
        dx = rail["x"];
        if (!profile->lookupRef(rail["reference"], x, y)) {
            x = y = 0;
            // Reference not found
        }
        gauge->addRail(profile, x + dx, y + dy);
    }
    return gauge;
}
