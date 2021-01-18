#ifndef TRAINS_RAILWAY_H
#define TRAINS_RAILWAY_H

#include "Renderable.h"
#include "Vector.h"

#include <list>

class TrackNode;
class TrackSection;
class Train;

// Track sections
// Signalling
// etc
class Railway : public Renderable
{
private:
    // TODO use a more lookup-efficient data structure
    std::list<TrackNode *> nodes;
    std::list<TrackSection *> sections;
    std::list<Train *> trains;

public:
    void addNode(TrackNode *node);
    void addSection(TrackSection *section);
    void addTrain(Train *train);

    // Find nearest node
    TrackNode *findClosestNode(const LineNormal3f &line, float range);

    void advance(float dt);

    RENDERABLE_GL();
};

#endif // TRAINS_RAILWAY_H
