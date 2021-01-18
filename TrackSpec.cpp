#include "TrackSpec.h"

TrackSpec::TrackSpec()
: trackGauge(nullptr),
  trackSpacing(1.0f),
  maxCurvature(1.0f / 10.0f),
  minCurvature(1.0f / 50.0f),
  maxCurvatureRate(1.0f / 10.0f / 10.0f),
  minCurvatureRate(1.0f / 50.0f / 50.0f)
{
}
