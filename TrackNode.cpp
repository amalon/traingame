#include "TrackNode.h"
#include "TrackSection.h"

TrackNode::TrackNode()
: position(0.0f),
  direction(0),
  curvature(0)
{
}

void TrackNode::notifySections()
{
    for (int i = 0; i < 2; ++i)
        for (TrackSection *section: sections[i])
            section->notifyNodeChanged(this);
}
