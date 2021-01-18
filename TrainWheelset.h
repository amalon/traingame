#ifndef TRAINS_TRAIN_WHEELSET_H
#define TRAINS_TRAIN_WHEELSET_H

#include "Renderable.h"
#include "TrackPosition.h"

#include "Vector.h"

class Gauge;

class TrainWheelset : public Renderable
{
private:
    // Radius of wheels at contact with rail (m)
    float wheelRadius;
    // Distance between contact with rails (m)
    float wheelGauge;
    // Conicity tan(angle)
    float conicity;
    // Mass of wheelset in (kg)
    float mass;

    // Current rotational angle of wheels (radians)
    float angle;
    // Angular speed of wheels in (radians / s)
    float angularSpeed;
    // Position forward/left of bogie center (m)
    Vec2f relativePosition;
    // Sideways slide movement range
    float slideRange;
    // Sideways slide movement spring constant
    float slideSpring;
    // Rolling resistance coefficient (force / weight to move)
    float resistance;
    // Coefficient of friction at the point of contact with the rail
    float friction;
    // Position on track
    TrackPosition trackPosition;

public:
    TrainWheelset(const Gauge *gauge, float radius, float position);
    virtual ~TrainWheelset();

    void moveAlongTrack(float distance);
    void reposition(const TrackPosition &newTrackPosition);

    const Vec2f &getRelativePosition() const
    {
        return relativePosition;
    }

    const TrackPosition &getTrackPosition() const
    {
        return trackPosition;
    }

    RENDERABLE_GL();
};

#endif // TRAINS_TRAIN_WHEELSET_H
