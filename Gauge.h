#ifndef TRAINS_GAUGE_H
#define TRAINS_GAUGE_H

#include <string>
#include <vector>
#include <ostream>

#include "RailProfile.h"

class Gauge
{
private:
    class RailPlacement {
    public:
        const RailProfile *profile;
        float x, y;
    };

    std::string name;
    float gauge;
    std::vector<RailPlacement> rails;

public:

    void setName(const std::string &newName)
    {
        name = newName;
    }

    void setGauge(float newGauge)
    {
        gauge = newGauge;
    }

    void addRail(const RailProfile *profile, float x, float y)
    {
        RailPlacement placement = { profile, x, y };
        rails.push_back(placement);
    }

    friend std::ostream &operator << (std::ostream &s, const Gauge &gauge) {
        s << "{ name: \"" << gauge.name << "\", gauge: " << gauge.gauge << ", rails = [";
        std::vector<RailPlacement>::const_iterator it;
        for (it = gauge.rails.begin(); it != gauge.rails.end(); ++it) {
            s << " { profile: " << *(*it).profile << ", x: " << (*it).x << ", y: " << (*it).y << " }";
        }
        return s << "] }";
    }
};

#endif // TRAINS_GAUGE_H
