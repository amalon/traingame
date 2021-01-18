#ifndef TRAINS_TRAIN_COUPLING_H
#define TRAINS_TRAIN_COUPLING_H

#include "Vector.h"

class TrainBogie;

class TrainCoupling
{
private:
    // Other coupling this is coupled to
    TrainCoupling *other;
    // The train chassis this coupling is connected to
    TrainBogie *parent;
    // Position relative to chassis
    Vec3f position;
    // Maximum tension (N) through the coupling
    float maxTension;
};

class TrainCouplingBuffersChain : public TrainCoupling
{
};

class TrainCouplingRigid : public TrainCoupling
{
};

#endif // TRAINS_TRAIN_COUPLING_H
