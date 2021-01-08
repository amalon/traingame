#include "TrackNode.h"
#include "TrackSection.h"
#include "TrackSpec.h"

#include <cmath>

TrackNode::TrackNode(const TrackSpec *newMinSpec)
: position(0.0f),
  direction(0),
  curvature(0),
  numTracks(1),
  minSpec(newMinSpec)
{
}

void TrackNode::notifySections()
{
    for (int i = 0; i < 2; ++i)
        for (TrackSection *section: sections[i])
            section->notifyNodeChanged(this);
}

void TrackNode::setMidpoint(const Vec3f &midpoint)
{
    Vec2f leftVec;
    sincosf(direction + M_PI/2, &leftVec[1], &leftVec[0]);
    float displacement = getMidpointOffset();
    setPosition(midpoint + (leftVec * displacement, 0.0f));
}

float TrackNode::getMidpointOffset() const
{
    return minSpec->getTrackSpacing() * (numTracks - 1);
}

float TrackNode::getMidpointOffset(int startTrack, int ofNumTracks) const
{
    return minSpec->getTrackSpacing() * (2 * startTrack + ofNumTracks - 1);
}

float TrackNode::getTrackOffset(int trackIndex) const
{
    return minSpec->getTrackSpacing() * (2 * trackIndex);
}

Vec3f TrackNode::getMidpoint(int startTrack, int ofNumTracks) const
{
    if (ofNumTracks < 0)
        ofNumTracks = numTracks;
    Vec2f rightVec;
    sincosf(direction - M_PI/2, &rightVec[1], &rightVec[0]);
    float displacement = getMidpointOffset(startTrack, ofNumTracks);
    return position + (rightVec * displacement, 0.0f);
}

Vec3f TrackNode::getPosition(int trackIndex) const
{
    if (trackIndex == 0) {
        return position;
    } else {
        Vec2f rightVec;
        sincosf(direction - M_PI/2, &rightVec[1], &rightVec[0]);
        float displacement = getTrackOffset(trackIndex);
        return position + (rightVec * displacement, 0.0f);
    }
}

float TrackNode::getCurvature(int trackIndex) const
{
    if (trackIndex == 0) {
        return curvature;
    } else {
        float displacement = minSpec->getTrackSpacing() * 2 * trackIndex;
        return 1.0f / (1.0f / curvature + displacement);
    }
}
