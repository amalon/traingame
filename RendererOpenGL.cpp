#include "RendererOpenGL.h"
#include "ControlMode.h"
#include "Railway.h"

#include <GL/gl.h>

RendererOpenGL::RendererOpenGL()
{
}

void RendererOpenGL::vRenderFrame()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup viewpoint
    setupViewpoint();
    // render 3d world & interface
    // render 2d interface

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);

    // FIXME Draw a dummy rectangle for now
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-10, -10);
    glVertex2f(-10, 10);
    glVertex2f(10, 10);
    glVertex2f(10, -10);
    glEnd();

    if (railway)
        renderObj(railway);

    if (navigateMode)
        renderObj(navigateMode);
    if (activeMode)
        renderObj(activeMode);
}

void RendererOpenGL::vSetViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

void RendererOpenGL::renderObj(Renderable *obj)
{
    obj->renderGL(this);
}

void RendererOpenGL::setupViewpoint()
{
    const Vec3f &target = viewpoint.getTarget();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(viewpoint.getLeft(), viewpoint.getRight(),
            viewpoint.getBottom(), viewpoint.getTop(),
            viewpoint.getNear(), viewpoint.getFar());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-target[0], -target[1], -target[2]);
}
