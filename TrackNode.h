#ifndef TRAINS_TRACK_NODE_H
#define TRAINS_TRACK_NODE_H

#include "TrackSpec.h"
#include "Vector.h"

#include <unordered_set>

class TrackSection;

class TrackNode
{
private:
    // Position of node
    Vec3f position;
    // Direction (radians CCW from east)
    float direction;
    // Curvature (radians/length CCW)
    float curvature;

    // Minimum track specifications
    const TrackSpec *minSpec;

    // Set of sections in each direction (backward, forward)
    std::unordered_set<TrackSection *> sections[2];

public:
    // Encapsulates a reference to a node in a particular direction (forwards or
    // backwards)
    class Reference
    {
    private:
        TrackNode *node;
        // True if the reference is to the forward end of the node
        bool forward;

    public:
        Reference()
        : node(nullptr)
        {
        }

        Reference(TrackNode *newNode, bool newForward)
        : node(newNode),
          forward(newForward)
        {
        }

        void set(TrackNode *newNode, bool newForward)
        {
            node = newNode;
            forward = newForward;
        }

        bool isForward() const
        {
            return forward;
        }

        const Vec3f &getPosition() const
        {
            return node->getPosition();
        }

        float getDirection() const
        {
            float direction = node->getDirection();
            if (forward)
                return direction;
            return direction + M_PI;
        }

        float getCurvature() const
        {
            float curvature = node->getCurvature();
            if (forward)
                return curvature;
            return -curvature;
        }

        const TrackSpec &getMinSpec() const
        {
            return node->getMinSpec();
        }

        void addTrackSection(TrackSection *section)
        {
            node->addTrackSection(section, forward);
        }
    };

public:
    // Constructor
    TrackNode(const TrackSpec *newMinSpec);

    // References
    Reference forward()
    {
        return Reference(this, true);
    }
    Reference backward()
    {
        return Reference(this, false);
    }

    // TrackSection linkage

    void addTrackSection(TrackSection *section, bool forward)
    {
        sections[forward].insert(section);
    }

    // Setters

    void notifySections();

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

    // Accessors

    const Vec3f &getPosition() const
    {
        return position;
    }

    float getDirection() const
    {
        return direction;
    }

    float getCurvature() const
    {
        return curvature;
    }

    const TrackSpec &getMinSpec() const
    {
        return *minSpec;
    }
};

#endif // TRAINS_TRACK_NODE_H
