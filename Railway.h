#ifndef TRAINS_RAILWAY_H
#define TRAINS_RAILWAY_H

#include "Renderable.h"
#include "TrackNode.h"
#include "TrackSection.h"
#include "Vector.h"

#include <list>

// Track sections
// Signalling
// etc
class Railway : public Renderable
{
private:
    // TODO use a more lookup-efficient data structure
    std::list<TrackNode *> nodes;
    std::list<TrackSection *> sections;

public:
    void addNode(TrackNode *node);
    void addSection(TrackSection *section);

    // Find nearest node
    TrackNode *findClosestNode(const LineNormal3f &line, float range);

    RENDERABLE_GL();
};

#endif // TRAINS_RAILWAY_H
