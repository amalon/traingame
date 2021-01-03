#include "Railway.h"
#include "RendererOpenGL.h"

#include <GL/gl.h>

void Railway::renderGL(RendererOpenGL *renderer)
{
    glLineWidth(2);
    glPointSize(4);
    for (TrackSection *section: sections) {
        renderer->renderObj(section);
    }

    glBegin(GL_POINTS);
    glColor3f(0.3f, 0.3f, 0.3f);
    for (const TrackNode *node: nodes)
        glVertex3fv((const float *)node->getPosition());
    glEnd();
}
