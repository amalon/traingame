#ifndef TRAINS_TRAIN_UNIT_H
#define TRAINS_TRAIN_UNIT_H

#include "Renderable.h"

class TrainBogie;
class TrackPosition;

// Consists of a collection of wheelsets, bogies and couplings that generally
// are kept together as a single unit
class TrainUnit : public Renderable
{
    TrainBogie *chassis;

public:
    TrainUnit();
    ~TrainUnit();

    void setChassis(TrainBogie *newChassis);

    void moveAlongTrack(float distance);
    void reposition(const TrackPosition &position);

    float getForwardExtent() const;
    float getBackwardExtent() const;
    float getLength() const;

    RENDERABLE_GL();
};

#endif // TRAINS_TRAIN_UNIT_H
