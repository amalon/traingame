#include "TrainUnit.h"
#include "TrainBogie.h"

TrainUnit::TrainUnit()
: chassis(nullptr)
{
}

TrainUnit::~TrainUnit()
{
    delete chassis;
}

void TrainUnit::setChassis(TrainBogie *newChassis)
{
    chassis = newChassis;
}

void TrainUnit::moveAlongTrack(float distance)
{
    chassis->moveAlongTrack(distance);
}

void TrainUnit::reposition(const TrackPosition &position)
{
    // Assume bogie is centered in unit for now
    chassis->reposition(position);
}

float TrainUnit::getForwardExtent() const
{
    return chassis->getForwardExtent();
}

float TrainUnit::getBackwardExtent() const
{
    return chassis->getBackwardExtent();
}

float TrainUnit::getLength() const
{
    return chassis->getLength();
}
