#ifndef TRAINS_TRACK_POSITION_H
#define TRAINS_TRACK_POSITION_H

#include "Vector.h"

class TrackNode;
class TrackSection;

class TrackPosition
{
private:
    // Section of track we're on
    const TrackSection *section;
    // Whether facing forward on that track section
    bool forward;
    // Which actual track we're on relative to section start node
    unsigned int trackIndex;
    // Distance along the track section (in section coordinates)
    float distance;

public:
    TrackPosition();

    void set(const TrackNode *node, bool newForward = true,
             unsigned int newTrackIndex = 0);
    void set(const TrackSection *newSection, bool newForward = true,
             unsigned int newTrackIndex = 0, float newDistance = 0);

    int getSectionTrackIndex() const;

    Vec3f getPosition(Mat22f *outRotMatrix = nullptr) const;

    // Face the other way
    void turnAround();

    operator bool () const
    {
        return section != nullptr;
    }

    bool atEnd(bool lookForward = true) const;

    // Find a TrackPosition moveDistance forward along the track
    TrackPosition operator + (float moveDistance) const;
    // Move the position moveDistance forward along the track
    TrackPosition &operator += (float moveDistance)
    {
        *this = *this + moveDistance;
        return *this;
    }

    // Find a TrackPosition moveDistance backward along the track
    TrackPosition operator - (float moveDistance) const
    {
        return *this + -moveDistance;
    }
    // Move the position moveDistance backward along the track
    TrackPosition &operator -= (float moveDistance)
    {
        return *this += -moveDistance;
    }

    // Advance to next section, return false if can't proceed
    bool advanceNextSection(bool lookForward = true);
    // Find the distance to the next section of track
    float distanceToSection(bool lookForward = true) const;
    // Find the distance to the next set of points or end
    float distanceToPoints(bool lookForward = true, float maxDistance = -1, const TrackNode **outNode = nullptr) const;
    // Find the distance to the end of the track (or points not set correctly)
    float distanceToEnd(bool lookForward = true, float maxDistance = -1, const TrackNode **outNode = nullptr) const;
};

#endif // TRAINS_TRACK_POSITION_H
