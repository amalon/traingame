#include "Train.h"
#include "TrainUnit.h"
#include "TrackPosition.h"

Train::Train()
: velocity(0)
{
}

Train::~Train()
{
    for (TrainUnit *unit: units)
        delete unit;
}

void Train::addUnitFront(TrainUnit *unit)
{
    units.push_back(unit);
}

void Train::addUnitBack(TrainUnit *unit)
{
    units.push_front(unit);
}

void Train::moveAlongTrack(float distance)
{
    position += distance;
    for (TrainUnit *unit: units)
        unit->moveAlongTrack(distance);
}

void Train::reposition(const TrackPosition &newPosition)
{
    float halfLength = getLength()/2;
    TrackPosition pos = newPosition;
    float leftover = 0;
    pos -= halfLength;
    position = pos + halfLength;
    for (TrainUnit *unit: units) {
        float forwardExtent = unit->getForwardExtent();
        float backwardExtent = unit->getBackwardExtent();
        pos += leftover + backwardExtent;
        leftover = forwardExtent + 1.0f;
        unit->reposition(pos);
    }
}

void Train::drive(float dt)
{
    constexpr float acceleration = 2.0f;
    constexpr float maxSpeed = 30.0f;
    if (velocity < 0) {
        velocity -= acceleration*dt;
        if (velocity < -maxSpeed)
            velocity = -maxSpeed;
    } else {
        velocity += acceleration*dt;
        if (velocity > maxSpeed)
            velocity = maxSpeed;
    }

    float len = getLength();
    if (velocity > 0) {
        if ((position + len/2).atEnd(true))
            velocity = -velocity/3;
    } else {
        if ((position - len/2).atEnd(false))
            velocity = -velocity/3;
    }

    moveAlongTrack(velocity * dt);
    reposition(position);
}

float Train::getLength() const
{
    float len = 0;
    for (TrainUnit *unit: units)
        len += unit->getLength();
    len += 1.0f * (units.size() - 1);
    return len;
}
