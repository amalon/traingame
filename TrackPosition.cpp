#include "TrackPosition.h"
#include "TrackSection.h"

#include <cassert>
#include <cmath>

TrackPosition::TrackPosition()
: section(nullptr),
  forward(true),
  trackIndex(0),
  distance(0)
{
}

void TrackPosition::set(const TrackNode *node, bool newForward,
                        unsigned int newTrackIndex)
{
    TrackNode::ConstReference ref(node, true, newTrackIndex, 1);
    if (!newForward)
        ref.reverse();

    // Move onto forward section
    section = ref.nextSection(0, &trackIndex, &forward);
    if (section) {
        distance = forward ? 0 : section->getLength(getSectionTrackIndex());
    } else {
        ref.reverse();
        section = ref.nextSection(0, &trackIndex, &forward);
        if (section) {
            distance = forward ? 0 : section->getLength(getSectionTrackIndex());
            forward = !forward;
        }
    }
}

void TrackPosition::set(const TrackSection *newSection, bool newForward,
                        unsigned int newTrackIndex, float newDistance)
{
    section = newSection;
    forward = newForward;
    trackIndex = newTrackIndex;
    distance = newDistance;
}

int TrackPosition::getSectionTrackIndex() const
{
    return section->start().parentTrackIndex(trackIndex);
}

Vec3f TrackPosition::getPosition() const
{
    assert(section && "Section must be set to get position");

    return section->getPosition(getSectionTrackIndex(), distance);
}

void TrackPosition::turnAround()
{
    forward = !forward;
}

bool TrackPosition::atEnd(bool lookForward) const
{
    if (distanceToSection(lookForward) > 0)
        return false;

    // Find end node, facing outwards
    const TrackNode::Reference *endNode;
    TrackNode::Reference nextNode;
    if (forward == lookForward) {
        endNode = &section->end();
        nextNode = endNode->reversed().subset(trackIndex, 1);
    } else {
        endNode = &section->start();
        nextNode = endNode->subset(trackIndex, 1).reversed();
    }
    // Check points are turned this way
    if (endNode->nextSection(trackIndex) != section) {
        return true;
    }
    // Find where points turn next
    TrackSection *next = nextNode.nextSection(0);
    return next == nullptr;
}

TrackPosition TrackPosition::operator + (float moveDistance) const
{
    if (!moveDistance)
        return *this;

    TrackPosition ret = *this;
    bool lookForward = (moveDistance > 0);
    float absDistance = fabs(moveDistance);
    while (absDistance > 0) {
        float toNext = ret.distanceToSection(lookForward);
        float dist = std::min(absDistance, toNext);
        if (ret.forward == lookForward)
            ret.distance += dist;
        else
            ret.distance -= dist;
        if (absDistance >= toNext) {
            if (!ret.advanceNextSection(lookForward))
                return ret;
        }
        absDistance -= toNext;
    }
    return ret;
}

bool TrackPosition::advanceNextSection(bool lookForward)
{
    // Find next node
    const TrackNode::Reference *endNode;
    TrackNode::Reference nextNode;
    if (forward == lookForward) {
        endNode = &section->end();
        nextNode = endNode->reversed().subset(trackIndex, 1);
    } else {
        endNode = &section->start();
        nextNode = endNode->subset(trackIndex, 1).reversed();
    }
    // Check points are turned this way
    if (endNode->nextSection(trackIndex) != section) {
        return false;
    }

    TrackSection *next = nextNode.nextSection(0, &trackIndex, &forward);
    if (!next)
        return false;
    section = next;
    distance = forward ? 0 : section->getLength(getSectionTrackIndex());
    if (!lookForward)
        forward = !forward;
    return true;
}

float TrackPosition::distanceToSection(bool lookForward) const
{
    if (!section)
        return 0;

    if (lookForward == forward) {
        // towards the end of section
        const float len = section->getLength(getSectionTrackIndex());
        return len - distance;
    } else {
        // towards the start of section
        return distance;
    }
}

float TrackPosition::distanceToPoints(bool lookForward, float maxDistance, const TrackNode **outNode) const
{
    return 0;
}

float TrackPosition::distanceToEnd(bool lookForward, float maxDistance, const TrackNode **outNode) const
{
    return 0;
}
