#ifndef TRAINS_TRACK_SPEC_H
#define TRAINS_TRACK_SPEC_H

#include "Gauge.h"

class TrackSpec
{
private:
    std::string name;

    const Gauge *trackGauge;

    // TODO LoadingGauge

    // TODO StructureGauge

    float trackSpacing;

public:
    // Constructors

    TrackSpec();

    // Setters

    void setName(const std::string &newName)
    {
        name = newName;
    }

    void setTrackGauge(const Gauge *newTrackGauge)
    {
        trackGauge = newTrackGauge;
    }

    void setTrackSpacing(float newTrackSpacing)
    {
        trackSpacing = newTrackSpacing;
    }

    // Getters

    const std::string &getName() const
    {
        return name;
    }

    const Gauge *getTrackGauge() const
    {
        return trackGauge;
    }

    float getTrackSpacing() const
    {
        return trackSpacing;
    }
};

#endif // TRAINS_TRACK_SPEC_H
