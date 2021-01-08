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
        Vec2f xy, dxy;
        dxy[0] = rail["x"];
        dxy[1] = 0;
        if (!profile->lookupRef(rail["reference"], xy[0], xy[1])) {
            xy.set(0);
            // Reference not found
        }
        gauge->addRail(profile, xy + dxy);
    }
    return gauge;
}
