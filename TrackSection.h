#ifndef TRAINS_TRACK_SECTION_H
#define TRAINS_TRACK_SECTION_H

#include "Renderable.h"
#include "TrackNode.h"
#include "ClothoidChain.h"

#include <list>

class TrackSection : public Renderable
{
private:
    typedef ClothoidChain<float, float> ClothoidChainT;
    typedef ClothoidChainT::Clothoid ClothoidT;

    // Nodes at each end
    TrackNode::Reference nodes[2];

    // Chain of clothoids describing track shape
    ClothoidChainT chain;

public:
    // Constructor
    TrackSection(TrackNode::Reference start,
                 TrackNode::Reference end);

    void interpolate();

    // Accessors
    const TrackNode::Reference &start() const
    {
        return nodes[0];
    }
    TrackNode::Reference &start()
    {
        return nodes[0];
    }

    const TrackNode::Reference &end() const
    {
        return nodes[1];
    }
    TrackNode::Reference &end()
    {
        return nodes[1];
    }

    // Notifications of node changes
    void notifyNodeChanged(TrackNode *node);

    RENDERABLE_GL();

private:

    static bool calcImpliedFromCurvatureRateAC(float directionDelta,
                                               float curvatureA, float curvatureB,
                                               float curvatureRateAC,
                                               float *outCurvatureRateCB,
                                               float *outLengthAC, float *outLengthCB);
    static bool interpolateClothoidPair(float directionA, float curvatureA,
                                        const Vec2f &positionB, float directionB,
                                        float curvatureB,
                                        float *outRate1, float *outLen1,
                                        float *outRate2, float *outLen2,
                                        Vec2f *outB, float *outExtraStraight);
};

#endif // TRAINS_TRACK_SECTION_H
