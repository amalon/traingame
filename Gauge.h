#ifndef TRAINS_GAUGE_H
#define TRAINS_GAUGE_H

#include <string>
#include <vector>
#include <ostream>

#include "RailProfile.h"
#include "Vector.h"

class Gauge
{
private:
    class RailPlacement {
    public:
        const RailProfile *profile;
        Vec2f position;
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

    void addRail(const RailProfile *profile, const Vec2f &position)
    {
        RailPlacement placement = { profile, position };
        rails.push_back(placement);
    }

    float getGauge() const
    {
        return gauge;
    }

    unsigned int getNumRails() const
    {
        return rails.size();
    }

    const Vec2f &getRailPosition(unsigned int rail) const
    {
        return rails[rail].position;
    }

    friend std::ostream &operator << (std::ostream &s, const Gauge &gauge) {
        s << "{ name: \"" << gauge.name << "\", gauge: " << gauge.gauge << ", rails = [";
        std::vector<RailPlacement>::const_iterator it;
        for (it = gauge.rails.begin(); it != gauge.rails.end(); ++it) {
            s << " { profile: " << *(*it).profile << ", x: " << (*it).position[0] << ", y: " << (*it).position[1] << " }";
        }
        return s << "] }";
    }
};

#endif // TRAINS_GAUGE_H
