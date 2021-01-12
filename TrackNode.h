#ifndef TRAINS_TRACK_NODE_H
#define TRAINS_TRACK_NODE_H

#include "TrackSpec.h"
#include "Vector.h"

#include <unordered_set>

class TrackSection;

class TrackNode
{
private:
    // Position of node at track 0
    Vec3f position;
    // Direction (radians CCW from east) of all tracks
    float direction;
    // Curvature (radians/length CCW) of track 0
    float curvature;
    // Number of tracks (numbered 0..numTracks-1) to right of position
    unsigned int numTracks;

    // Minimum track specifications
    const TrackSpec *minSpec;

    // Set of sections in each direction (backward, forward)
    std::unordered_set<TrackSection *> sections[2];

public:
    // Encapsulates a reference to a node in a particular direction (forwards or
    // backwards)
    template <typename T>
    class TReference
    {
    private:
        T node;
        // True if the reference is to the forward end of the node
        bool forward;
        // Parent node track index of reference trackIndex 0
        int firstTrack;
        // Number of tracks
        unsigned int numTracks;


    public:
        TReference()
        : node(nullptr)
        {
        }

        template <typename U>
        TReference(const TReference<const U*> &other)
        : node(other.node),
          forward(other.forward),
          firstTrack(other.firstTrack),
          numTracks(other.numTracks)
        {
        }

        TReference(T newNode, bool newForward)
        : node(newNode),
          forward(newForward),
          firstTrack(newForward ? 0 : (newNode->numTracks - 1)),
          numTracks(newNode->numTracks)
        {
        }

        TReference(T newNode, bool newForward,
                  int newFirstTrack, unsigned int newNumTracks)
        : node(newNode),
          forward(newForward),
          firstTrack(newFirstTrack),
          numTracks(newNumTracks)
        {
        }

        void set(T newNode, bool newForward)
        {
            node = newNode;
            forward = newForward;
            firstTrack = newForward ? 0 : (newNode->numTracks - 1);
            numTracks = newNode->numTracks;
        }

        void set(T newNode, bool newForward,
                 int newFirstTrack, int newNumTracks)
        {
            node = newNode;
            forward = newForward;
            firstTrack = newFirstTrack;
            numTracks = newNumTracks;
        }

        bool isForward() const
        {
            return forward;
        }

        // Get offset of midpoint of subset of tracks relative to position
        float getMidpointOffset() const
        {
            if (forward)
                return node->getMidpointOffset(firstTrack, numTracks);
            else
                return -node->getMidpointOffset(firstTrack - (numTracks - 1), numTracks);
        }

        float getTrackOffset(int trackIndex) const
        {
            float offset = node->getTrackOffset(parentTrackIndex(trackIndex));
            if (forward)
                return offset;
            else
                return -offset;
        }

        Vec3f getMidpoint() const
        {
            if (forward)
                return node->getMidpoint(firstTrack, numTracks);
            else
                return node->getMidpoint(firstTrack - (numTracks - 1), numTracks);
        }

        Vec3f getPosition(int trackIndex = 0) const
        {
            return node->getPosition(parentTrackIndex(trackIndex));
        }

        float getDirection() const
        {
            float direction = node->getDirection();
            if (forward)
                return direction;
            return direction + M_PI;
        }

        float getCurvature(int trackIndex = 0) const
        {
            float curvature = node->getCurvature(parentTrackIndex(trackIndex));
            if (forward)
                return curvature;
            return -curvature;
        }

        unsigned int getNumTracks() const
        {
            return node->getNumTracks();
        }

        const TrackSpec &getMinSpec() const
        {
            return node->getMinSpec();
        }

        void addTrackSection(TrackSection *section)
        {
            node->addTrackSection(section, forward);
        }

    private:
        int parentTrackIndex(int trackIndex) const
        {
            return firstTrack + (forward ? trackIndex : -trackIndex);
        }
    };

    typedef TReference<TrackNode *> Reference;
    typedef TReference<const TrackNode *> ConstReference;

public:
    // Constructor
    TrackNode(const TrackSpec *newMinSpec);

    // References
    Reference forward()
    {
        return Reference(this, true);
    }
    ConstReference forward() const
    {
        return ConstReference(this, true);
    }

    Reference forward(int firstTrack, int numTracks)
    {
        return Reference(this, true, firstTrack, numTracks);
    }
    ConstReference forward(int firstTrack, int numTracks) const
    {
        return ConstReference(this, true, firstTrack, numTracks);
    }

    Reference backward()
    {
        return Reference(this, false);
    }
    ConstReference backward() const
    {
        return ConstReference(this, false);
    }

    Reference backward(int firstTrack, int numTracks)
    {
        return Reference(this, false, firstTrack, numTracks);
    }
    ConstReference backward(int firstTrack, int numTracks) const
    {
        return ConstReference(this, false, firstTrack, numTracks);
    }

    // TrackSection linkage

    void addTrackSection(TrackSection *section, bool forward)
    {
        sections[forward].insert(section);
    }

    // Setters

    void notifySections();

    void setMidpoint(const Vec3f &midpoint);

    void setPosition(const Vec3f &newPosition)
    {
        position = newPosition;
        notifySections();
    }

    void setDirection(float newDirection)
    {
        direction = newDirection;
        notifySections();
    }

    void setCurvature(float newCurvature)
    {
        curvature = newCurvature;
        notifySections();
    }

    void setNumTracks(unsigned int newNumTracks)
    {
        numTracks = newNumTracks;
    }

    // Accessors

    // Get offset of midpoint relative to position
    float getMidpointOffset() const;
    // Get offset of midpoint of subset of tracks relative to position
    float getMidpointOffset(int startTrack, int ofNumTracks) const;
    // Get offset of track relative to position
    float getTrackOffset(int trackIndex) const;

    Vec3f getMidpoint(int startTrack = 0, int ofNumTracks = -1) const;

    Vec3f getPosition(int trackIndex = 0) const;

    float getDirection() const
    {
        return direction;
    }

    float getCurvature(int trackIndex = 0) const;

    unsigned int getNumTracks() const
    {
        return numTracks;
    }

    const TrackSpec &getMinSpec() const
    {
        return *minSpec;
    }
};

#endif // TRAINS_TRACK_NODE_H
