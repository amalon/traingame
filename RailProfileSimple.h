#ifndef TRAINS_RAIL_PROFILE_SIMPLE_H
#define TRAINS_RAIL_PROFILE_SIMPLE_H

#include "RailProfile.h"

// Simple rail profile modelled as rectangular foot, web, head
class RailProfileSimple : public RailProfile
{
public:
    enum Level {
        FOOT = 0,
        WEB,
        HEAD,
        LEVELS
    };

private:
    float width[LEVELS];
    float height[LEVELS];

public:
    RailProfileSimple()
    : width(),
      height()
    {
    }

    void setBlockSize(Level level, float blockWidth, float blockHeight)
    {
        width[level] = blockWidth;
        height[level] = blockHeight;
    }

protected:

};

#endif // TRAINS_RAIL_PROFILE_SIMPLE_H
