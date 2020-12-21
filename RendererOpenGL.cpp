#include "RendererOpenGL.h"

#include <GL/gl.h>

void RendererOpenGL::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

void RendererOpenGL::renderFrame()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup viewpoint
    // render 3d world & interface
    // render 2d interface
}
