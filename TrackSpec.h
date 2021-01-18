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

    // Absolute maximum curvature (rad/m)
    float maxCurvature;
    // Preferred (minimum) full speed curvature (rad/m)
    float minCurvature;

    // Maximum curvature rate (rad/m²)
    float maxCurvatureRate;
    // Preferred (minimum) curvature rate (rad/m²)
    float minCurvatureRate;

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

    void setMaxCurvature(float newMaxCurvature)
    {
        maxCurvature = newMaxCurvature;
    }
    void setMinCurvature(float newMinCurvature)
    {
        minCurvature = newMinCurvature;
    }

    void setMaxCurvatureRate(float newMaxCurvatureRate)
    {
        maxCurvatureRate = newMaxCurvatureRate;
    }
    void setMinCurvatureRate(float newMinCurvatureRate)
    {
        minCurvatureRate = newMinCurvatureRate;
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

    float getMaxCurvature() const
    {
        return maxCurvature;
    }
    float getMinCurvature() const
    {
        return minCurvature;
    }

    float getMaxCurvatureRate() const
    {
        return maxCurvatureRate;
    }
    float getMinCurvatureRate() const
    {
        return minCurvatureRate;
    }
};

#endif // TRAINS_TRACK_SPEC_H
