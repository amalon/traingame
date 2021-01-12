#ifndef TRAINS_TRACK_NODE_H
#define TRAINS_TRACK_NODE_H

#include "TrackSpec.h"
#include "Vector.h"

#include <cassert>
#include <unordered_set>

class TrackSection;

class TrackNode
{
private:
    // Limit the number of tracks coming out of one side of a node
    static constexpr unsigned int maxDivergence = 2;

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

    // When referring to track sections, we refer to a specific end
    class SectionRef {
    public:
        TrackSection *section;
        bool forward;

        SectionRef()
        : section(nullptr),
          forward(true)
        {
        }

        void set(TrackSection *newSection, bool newForward)
        {
            section = newSection;
            forward = newForward;
        }
    };

    // We'll have one of these for each track in each direction
    class TrackDirectionInfo {
    public:
        // Up to 3 sections leading off this track
        SectionRef sections[maxDivergence];
        // The number of sections
        unsigned char numSections;
        // The current setting of the points
        unsigned char defaultIndex;

        TrackDirectionInfo()
        : numSections(0),
          defaultIndex(0)
        {
        }

        // Find whether there's space for the given section
        bool hasSpaceForSection(const TrackSection *section) const
        {
            // Space in array?
            if (numSections < maxDivergence)
                return true;
            // Or already in array?
            for (const SectionRef &ref: sections)
                if (ref.section == section)
                    return true;
            // Otherwise it won't fit
            return false;
        }

        // Try adding track section
        bool addSection(TrackSection *section, bool forward)
        {
            // Already in array?
            for (const SectionRef &ref: sections)
                if (ref.section == section)
                    return true;
            // Space in array?
            if (numSections < maxDivergence) {
                sections[numSections++].set(section, forward);
                return true;
            }
            return false;
        }

        SectionRef &defaultSection()
        {
            return sections[defaultIndex];
        }

        void switchPoints()
        {
            unsigned int next = defaultIndex + 1;
            if (next >= numSections)
                next = 0;
            defaultIndex = next;
        }
    };

    // We'll have one of these for each track
    class TrackInfo {
    public:
        // 0 = backwards, 1 = forwards
        TrackDirectionInfo directionInfo[2];
    };

    // Track information
    TrackInfo *trackInfo;

    // Set of sections
    std::unordered_set<TrackSection *> allSections;

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
            assert(numTracks > 0);
            assert(numTracks <= node->getNumTracks());
            assert(firstTrack >= 0);
            assert(firstTrack < node->getNumTracks());
            assert(firstTrack + (forward ? (numTracks-1) : (1-numTracks)) >= 0);
            assert(firstTrack + (forward ? (numTracks-1) : (1-numTracks)) < node->getNumTracks());
        }

        TReference subset(unsigned int first, unsigned int count) const
        {
            return TReference(node, forward,
                              firstTrack + first,
                              count);
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

        TReference reversed() const
        {
            return TReference(node, !forward,
                              forward ? firstTrack + numTracks - 1
                                      : firstTrack - numTracks + 1,
                              numTracks);
        }
        TReference &reverse()
        {
            return *this = reversed();
        }

        bool isForward() const
        {
            return forward;
        }

        int getFirstTrack() const
        {
            return firstTrack;
        }

        unsigned int getNumTracks() const
        {
            return numTracks;
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

        const TrackSpec &getMinSpec() const
        {
            return node->getMinSpec();
        }

        // return false on failure
        bool addTrackSection(TrackSection *section, bool nextForward);

        TrackSection *nextSection(unsigned int trackIndex,
                                  unsigned int *outTrackIndex = nullptr,
                                  bool *outForward = nullptr) const;

        int parentTrackIndex(int trackIndex) const
        {
            return firstTrack + (forward ? trackIndex : -trackIndex);
        }
        int localTrackIndex(int parentTrackIndex) const
        {
            return forward ? (parentTrackIndex - firstTrack)
                           : (firstTrack - parentTrackIndex);
        }
    };

    typedef TReference<TrackNode *> Reference;
    typedef TReference<const TrackNode *> ConstReference;

public:
    // Constructor
    TrackNode(const TrackSpec *newMinSpec);
    // Destructor
    ~TrackNode();

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

    // return false if track couldn't be added
    bool addTrackSection(bool forward, int startTrack, int ofNumTracks,
                         TrackSection *section, bool nextForward);

    bool hasPoints(unsigned int trackIndex, bool forward) const;
    void switchPoints(unsigned int trackIndex, bool forward);

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

    void setNumTracks(unsigned int newNumTracks);

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
