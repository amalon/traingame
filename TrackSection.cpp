#include "TrackSection.h"

TrackSection::TrackSection(TrackNode::Reference start,
                           TrackNode::Reference end)
: nodes{start, end}
{
}
