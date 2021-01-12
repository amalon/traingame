#include "TrackNode.h"
#include "TrackSection.h"
#include "TrackSpec.h"

#include <cassert>
#include <cmath>
#include <cstring>

template class TrackNode::TReference<TrackNode *>;
template class TrackNode::TReference<const TrackNode *>;

template <>
bool TrackNode::TReference<TrackNode *>::addTrackSection(TrackSection *section, bool nextForward)
{
    int startTrack = firstTrack;
    if (!forward)
        startTrack -= numTracks - 1;
    return node->addTrackSection(forward, startTrack, numTracks,
                                    section, nextForward);
}

TrackNode::TrackNode(const TrackSpec *newMinSpec)
: position(0.0f),
  direction(0),
  curvature(0),
  numTracks(1),
  minSpec(newMinSpec),
  trackInfo(nullptr)
{
}

TrackNode::~TrackNode()
{
    delete [] trackInfo;
}

bool TrackNode::addTrackSection(bool forward, int startTrack, int ofNumTracks,
                                TrackSection *section, bool nextForward)
{
    allSections.insert(section);

    if (startTrack < 0 || startTrack + ofNumTracks > numTracks)
        return false;

    if (!trackInfo)
        trackInfo = new TrackInfo[numTracks];

    // Check all the tracks have space
    for (int i = startTrack; i < startTrack + ofNumTracks; ++i)
        if (!trackInfo[i].directionInfo[forward ? 1 : 0].hasSpaceForSection(section))
            return false;

    // Now insert section
    for (int i = startTrack; i < startTrack + ofNumTracks; ++i) {
        bool ret = trackInfo[i].directionInfo[forward ? 1 : 0].addSection(section, nextForward);
        assert(ret && "Unexpectedly failed to add section");
    }

    return true;
}

void TrackNode::notifySections()
{
    for (int i = 0; i < 2; ++i)
        for (TrackSection *section: allSections)
            section->notifyNodeChanged(this);
}

void TrackNode::setMidpoint(const Vec3f &midpoint)
{
    Vec2f leftVec;
    sincosf(direction + M_PI/2, &leftVec[1], &leftVec[0]);
    float displacement = getMidpointOffset();
    setPosition(midpoint + (leftVec * displacement, 0.0f));
}

void TrackNode::setNumTracks(unsigned int newNumTracks)
{
    if (numTracks == newNumTracks)
        return;

    if (!trackInfo || newNumTracks > numTracks) {
        TrackInfo *newTrackInfo = new TrackInfo[newNumTracks];
        if (trackInfo)
            memcpy(newTrackInfo, trackInfo, sizeof(newTrackInfo[0])*numTracks);
        delete [] trackInfo;
        trackInfo = newTrackInfo;
    }

    numTracks = newNumTracks;
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
