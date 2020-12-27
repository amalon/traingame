#ifndef TRAINS_TRACK_NODE_H
#define TRAINS_TRACK_NODE_H

#include "Vector.h"

class TrackNode
{
private:
    // Position of node
    Vec3f position;
    // Direction (radians CCW from east)
    float direction;
    // Curvature (radians/length CCW)
    float curvature;

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
    };

public:
    // Constructor
    TrackNode();

    // References
    Reference forward()
    {
        return Reference(this, true);
    }
    Reference backward()
    {
        return Reference(this, false);
    }

    // Setters

    void setPosition(const Vec3f &newPosition)
    {
        position = newPosition;
    }

    void setDirection(float newDirection)
    {
        direction = newDirection;
    }

    void setCurvature(float newCurvature)
    {
        curvature = newCurvature;
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

};

#endif // TRAINS_TRACK_NODE_H
