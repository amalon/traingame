#include "RendererOpenGL.h"

#include <GL/gl.h>

RendererOpenGL::RendererOpenGL()
: aspect(1)
{
}

void RendererOpenGL::setViewport(int width, int height)
{
    if (height)
        aspect = (float)width / height;
    glViewport(0, 0, width, height);
}

void RendererOpenGL::renderFrame()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup viewpoint
    setupViewpoint();
    // render 3d world & interface
    // render 2d interface

    // FIXME Draw a dummy rectangle for now
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-10, -10);
    glVertex2f(-10, 10);
    glVertex2f(10, 10);
    glVertex2f(10, -10);
    glEnd();
}

void RendererOpenGL::setupViewpoint()
{
    const float *target = viewpoint.getTarget();
    float dist = viewpoint.getDistance();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-dist * aspect, dist * aspect,
            -dist, dist,
            dist, -100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-target[0], -target[1], -target[2]);
}
