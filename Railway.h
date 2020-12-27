#ifndef TRAINS_RAILWAY_H
#define TRAINS_RAILWAY_H

#include "TrackNode.h"
#include "TrackSection.h"

#include <list>

// Track sections
// Signalling
// etc
class Railway
{
private:
    // TODO use a more lookup-efficient data structure
    std::list<TrackNode *> nodes;
    std::list<TrackSection *> sections;

public:
    void addNode(TrackNode *node);
    void addSection(TrackSection *section);

    void render();
};

#endif // TRAINS_RAILWAY_H
