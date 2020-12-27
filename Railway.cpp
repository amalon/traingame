#include "Railway.h"

#include <GL/gl.h>

void Railway::addNode(TrackNode *node)
{
    nodes.push_back(node);
}

void Railway::addSection(TrackSection *section)
{
    sections.push_back(section);
}

// TODO Move this to OpenGL code
void Railway::render()
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
