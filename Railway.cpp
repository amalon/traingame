#include "Railway.h"
#include "TrackNode.h"

void Railway::addNode(TrackNode *node)
{
    nodes.push_back(node);
}

void Railway::addSection(TrackSection *section)
{
    sections.push_back(section);
}

// Find nearest node
TrackNode *Railway::findClosestNode(const LineNormal3f &line, float range)
{
    TrackNode *best = nullptr;
    float bestRangeSqr = range * range;
    for (TrackNode *node: nodes) {
        Vec3f closest = line * line.closestPoint(node->getPosition());
        float distanceSqr = (closest - node->getPosition()).sqr();
        if (distanceSqr < bestRangeSqr) {
            best = node;
            bestRangeSqr = distanceSqr;
        }
    }
    return best;
}
