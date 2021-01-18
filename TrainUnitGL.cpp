#include "TrainUnit.h"
#include "TrainBogie.h"
#include "RendererOpenGL.h"

#include <GL/gl.h>

void TrainUnit::renderGL(RendererOpenGL *renderer)
{
    renderer->renderObj(chassis);
}
