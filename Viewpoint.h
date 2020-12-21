#ifndef TRAINS_VIEWPOINT_H
#define TRAINS_VIEWPOINT_H

class Viewpoint
{
private:
    /// Target coordinates
    float target[3];
    /// Distance
    float distance;

public:
    Viewpoint();

    void pan(float dx, float dy);
    void zoom(float ddist);

    const float *getTarget() const
    {
        return target;
    }

    float getDistance() const
    {
        return distance;
    }
};

#endif // TRAINS_VIEWPOINT_H
