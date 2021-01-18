#ifndef TRAINS_TRAIN_H
#define TRAINS_TRAIN_H

#include "Renderable.h"
#include "TrackPosition.h"

#include <list>

class TrackPosition;
class TrainUnit;

// Consists of a number of TrainUnits coupled together into a Train
class Train : public Renderable
{
    std::list<TrainUnit *> units;
    TrackPosition position;
    float velocity;

public:
    Train();
    ~Train();

    void addUnitFront(TrainUnit *unit);
    void addUnitBack(TrainUnit *unit);

    void moveAlongTrack(float distance);
    void reposition(const TrackPosition &newPosition);

    void drive(float dt);

    float getLength() const;

    RENDERABLE_GL();
};

#endif // TRAINS_TRAIN_H
