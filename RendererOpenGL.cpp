#include "RendererOpenGL.h"
#include "ControlMode.h"
#include "Railway.h"

#include <GL/gl.h>

RendererOpenGL::RendererOpenGL()
{
}

void RendererOpenGL::renderObj(Renderable *obj)
{
    obj->renderGL(this);
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

    // TODO Load a better island from somewhere
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 256; ++i) {
        float vec[2];
        sincosf(M_PI*2*i/256, &vec[0], &vec[1]);
        float rad = 1000 + 200*sin(M_PI*20*i/256);
        glVertex2f(rad*vec[0], rad*vec[1]);
    }
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
