#include "TrainWheelset.h"
#include "Gauge.h"

#include <cmath>

TrainWheelset::TrainWheelset(const Gauge *gauge, float radius, float position)
: wheelRadius(radius),
  wheelGauge(gauge->getGauge()),
  conicity(1.0f/20),
  mass(radius * radius * M_PI * 0.1f * 8000), // based on density of steel
  angle(0),
  angularSpeed(0),
  relativePosition(position, 0),
  slideRange(0),
  slideSpring(0),
  resistance(0),
  friction(0)
{
}

TrainWheelset::~TrainWheelset()
{
}

void TrainWheelset::moveAlongTrack(float distance)
{
    trackPosition += distance;
}

void TrainWheelset::reposition(const TrackPosition &newTrackPosition)
{
    trackPosition = newTrackPosition;
}
