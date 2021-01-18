#include "TrainBogie.h"
#include "TrainWheelset.h"
#include "TrainCoupling.h"

#include <cassert>

TrainBogie::TrainBogie()
: parents{},
  mass(1000),
  forwardSpeed(0),
  velocity(0.0f),
  position(0.0f),
  direction(0),
  width(2),
  forwardExtent(1),
  backwardExtent(1),
  centerHeight(1)
{
}

TrainBogie::~TrainBogie()
{
    for (TrainWheelset *wheelset: wheelsets)
        delete wheelset;
    for (TrainCoupling *coupling: couplings)
        delete coupling;
}

void TrainBogie::initWheelsets(unsigned int count, const Gauge *gauge,
                               float radius, float length)
{
    float startPos = 0, deltaPos = 0;
    if (count >= 2) {
        startPos = -length/2;
        deltaPos = length / (count - 1);
    }
    assert(wheelsets.empty() && "Wheelsets should be empty");
    wheelsets.reserve(count);
    for (int i = 0; i < count; ++i) {
        float pos = startPos + deltaPos * i;
        TrainWheelset *wheelset = new TrainWheelset(gauge, radius, pos);
        wheelsets.push_back(wheelset);
    }

    forwardExtent = length/2 + radius*2;
    backwardExtent = forwardExtent;
}

void TrainBogie::addWheelset(TrainWheelset *wheelset)
{
    wheelsets.push_back(wheelset);
}

void TrainBogie::addBogie(TrainBogie *bogie, const Vec3f &bogiePosition)
{
    struct BogieConnection bogieConnection;
    bogieConnection.bogie = bogie;
    bogieConnection.position = bogiePosition;
    bogies.push_back(bogieConnection);

    if (bogiePosition[0] + bogie->getForwardExtent() > forwardExtent)
        forwardExtent = bogiePosition[0] + bogie->getForwardExtent();
    if (bogiePosition[0] - bogie->getBackwardExtent() < -backwardExtent)
        backwardExtent = -bogiePosition[0] + bogie->getBackwardExtent();
}

void TrainBogie::calcPosition()
{
    Vec3f pos1, pos2;
    float rel1, rel2;
    if (wheelsets.size() > 1) {
        TrainWheelset *wheelset1 = wheelsets[0];
        TrainWheelset *wheelset2 = wheelsets[wheelsets.size() - 1];
        pos1 = wheelset1->getTrackPosition().getPosition();
        pos2 = wheelset2->getTrackPosition().getPosition();
        rel1 = wheelset1->getRelativePosition()[0];
        rel2 = wheelset2->getRelativePosition()[0];
    } else if (bogies.size() > 1) {
        // If there are multiple bogies, position & orientation is determined from them
        const BogieConnection &bogie1 = bogies[0];
        const BogieConnection &bogie2 = bogies[bogies.size() - 1];
        pos1 = bogie1.bogie->getPosition();
        pos2 = bogie2.bogie->getPosition();
        rel1 = bogie1.position[0];
        rel2 = bogie2.position[0];
    } else if (bogies.size() && wheelsets.size()) {
        // There is a bogie and a wheelset, use them
        TrainWheelset *wheelset = wheelsets[0];
        const BogieConnection &bogie = bogies[0];
        pos1 = wheelset->getTrackPosition().getPosition();
        pos2 = bogie.bogie->getPosition();
        rel1 = wheelset->getRelativePosition()[0];
        rel2 = bogie.position[0];
    } else if (bogies.size()) {
        // There is at least child bogie, assume fixed and use that
        const BogieConnection &bogie = bogies[0];
        pos1 = pos2 = bogie.bogie->getPosition();
        rel1 = rel2 = bogie.position[0];
    } else if (wheelsets.size()) {
        // There is at least a single wheelset, assume fixed and use that
        TrainWheelset *wheelset = wheelsets[0];
        pos1 = pos2 = wheelset->getTrackPosition().getPosition();
        rel1 = rel2 = wheelset->getRelativePosition()[0];
    } else {
        // Eek, nothing to go on
        return;
    }

    float divis = rel1 - rel2;
    if (divis)
        position = (pos2*rel1 - pos1*rel2) / divis;
    else
        position = pos1;

    Vec3f vec = pos2 - pos1;
    float dist = vec.mag();
    if (dist > 0) {
        Vec3f vecNorm = vec / dist;
        direction = atan2(vecNorm[1], vecNorm[0]);
    }
}

void TrainBogie::moveAlongTrack(float distance)
{
    // TODO Gotta be a bit smarter here
    for (TrainWheelset *wheelset: wheelsets)
        wheelset->moveAlongTrack(distance);
    for (BogieConnection &bogie: bogies)
        bogie.bogie->moveAlongTrack(distance);
    calcPosition();
}

void TrainBogie::reposition(const TrackPosition &position)
{
    for (TrainWheelset *wheelset: wheelsets)
        wheelset->reposition(position + wheelset->getRelativePosition()[0]);
    for (BogieConnection &bogie: bogies)
        bogie.bogie->reposition(position + bogie.position[0]);
    calcPosition();
}
