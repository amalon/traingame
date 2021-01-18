#ifndef TRAINS_TRAIN_BOGIE_H
#define TRAINS_TRAIN_BOGIE_H

#include "Renderable.h"
#include "Vector.h"

#include <vector>

class Gauge;
class TrackPosition;
class TrainWheelset;
class TrainCoupling;

class TrainBogie : public Renderable
{
private:
    struct BogieConnection {
        TrainBogie *bogie;
        // Position of bogie pivot/contact point
        Vec3f position;
    };

    // Fixed wheelsets on bogie
    std::vector<TrainWheelset *> wheelsets;
    // Child bogies
    std::vector<struct BogieConnection> bogies;
    // Parent bogie (up to 2)
    TrainBogie *parents[2];
    // Mass of bogie (excluding wheelsets) (kg)
    float mass;
    // Forward speed of bogie (m/s)
    float forwardSpeed;
    // Current 3D velocity of bogie
    Vec3f velocity;
    // Current 3D location of bogie
    Vec3f position;
    // Current forward direction (radians CCW from east)
    float direction;
    // Width of bogie
    float width;
    // Maximum extents
    float forwardExtent;
    float backwardExtent;
    // Height of center contact with parent bogie
    float centerHeight;
    // Couplings
    std::vector<TrainCoupling *> couplings;

public:
    TrainBogie();
    virtual ~TrainBogie();

    const Vec3f &getPosition() const
    {
        return position;
    }
    float getDirection() const
    {
        return direction;
    }
    float getForwardExtent() const
    {
        return forwardExtent;
    }
    float getBackwardExtent() const
    {
        return backwardExtent;
    }
    float getLength() const
    {
        return forwardExtent + backwardExtent;
    }

    // Initialise wheelsets
    void initWheelsets(unsigned int count, const Gauge *gauge, float radius,
                       float length);

    void setExtent(float backward, float forward)
    {
        backwardExtent = backward;
        forwardExtent = forward;
    }
    void setLength(float length)
    {
        backwardExtent = forwardExtent = length / 2;
    }

    // Add a single wheelset
    void addWheelset(TrainWheelset *wheelset);
    // Add a pivotting bogie
    void addBogie(TrainBogie *bogie, const Vec3f &bogiePosition);

    void calcPosition();
    void moveAlongTrack(float distance);
    void reposition(const TrackPosition &position);

    RENDERABLE_GL();
};

#endif // TRAINS_TRAIN_BOGIE_H
