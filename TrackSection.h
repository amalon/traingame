#ifndef TRAINS_TRACK_SECTION_H
#define TRAINS_TRACK_SECTION_H

#include "TrackNode.h"

class TrackSection
{
private:
    // Nodes at each end
    TrackNode::Reference nodes[2];

public:
    // Constructor
    TrackSection(TrackNode::Reference start,
                 TrackNode::Reference end);

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

};

#endif // TRAINS_TRACK_SECTION_H
