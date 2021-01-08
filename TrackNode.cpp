#include "TrackNode.h"
#include "TrackSection.h"
#include "TrackSpec.h"

TrackNode::TrackNode(const TrackSpec *newMinSpec)
: position(0.0f),
  direction(0),
  curvature(0),
  minSpec(newMinSpec)
{
}

void TrackNode::notifySections()
{
    for (int i = 0; i < 2; ++i)
        for (TrackSection *section: sections[i])
            section->notifyNodeChanged(this);
}
