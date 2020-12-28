#include "Renderable.h"
#include "RendererOpenGL.h"

void Renderable::render(Renderer *renderer)
{
    // nothing to fall back to
}

void Renderable::renderGL(RendererOpenGL *renderer)
{
    // fall back to generic render
    render((Renderer *)renderer);
}
