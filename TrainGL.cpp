#include "Train.h"
#include "TrainUnit.h"
#include "RendererOpenGL.h"

#include <GL/gl.h>

void Train::renderGL(RendererOpenGL *renderer)
{
    for (TrainUnit *unit: units)
        renderer->renderObj(unit);
}
