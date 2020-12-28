#include "Railway.h"
#include <GL/gl.h>

void Railway::renderGL(RendererOpenGL *renderer)
{
    glLineWidth(2);
    glPointSize(4);

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    for (const TrackSection *section: sections) {
        glVertex3fv((const float *)section->start().getPosition());
        glVertex3fv((const float *)section->end().getPosition());
    }
    glEnd();

    glBegin(GL_POINTS);
    glColor3f(0.3f, 0.3f, 0.3f);
    for (const TrackNode *node: nodes)
        glVertex3fv((const float *)node->getPosition());
    glEnd();
}
