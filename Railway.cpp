#include "Railway.h"
#include "TrackNode.h"
#include "Train.h"

void Railway::addNode(TrackNode *node)
{
    nodes.push_back(node);
}

void Railway::addSection(TrackSection *section)
{
    sections.push_back(section);
}

void Railway::addTrain(Train *train)
{
    trains.push_back(train);
}

// Find nearest node
TrackNode *Railway::findClosestNode(const LineNormal3f &line, float range)
{
    TrackNode *best = nullptr;
    float bestRangeSqr = range * range;
    for (TrackNode *node: nodes) {
        Vec3f closest = line * line.closestPoint(node->getMidpoint());
        float distanceSqr = (closest - node->getMidpoint()).sqr();
        if (distanceSqr < bestRangeSqr) {
            best = node;
            bestRangeSqr = distanceSqr;
        }
    }
    return best;
}

void Railway::advance(float dt)
{
    for (Train *train: trains)
        train->drive(dt);
}
