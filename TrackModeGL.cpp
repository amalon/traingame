#include "TrackMode.h"
#include "RendererOpenGL.h"
#include "Clothoid.h"

#include <GL/gl.h>

static void renderTrackNode(const Renderer *renderer, const TrackNode *node)
{
    // Draw a circle around the node
    glBegin(GL_LINE_LOOP);
    float size = renderer->getViewpoint().viewportToWorldSize(node->getPosition(), 0.02);
    const Vec3f &position = node->getPosition();
    for (int i = 0; i < 32; ++i) {
        float theta = M_PI * 2 * i / 32;
        float sin, cos;
        sincosf(theta, &sin, &cos);
        glVertex3f(position[0] + size * sin,
                    position[1] + size * cos,
                    position[2]);
    }
    glEnd();

    // Draw a line perpendicular to the direction across the node
    float direction = node->getDirection();
    Vec3f directionVector;
    sincosf(direction + M_PI/2, &directionVector[1], &directionVector[0]);
    directionVector[2] = 0;
    glBegin(GL_LINES);
    glVertex3fv((const float *)(position + 3 * size * directionVector));
    glVertex3fv((const float *)(position - 3 * size * directionVector));
    glEnd();

    // Draw a curve showing the curvature through the node
    Clothoid<float, float> clothoid;
    clothoid.setStartPosition((Vec2f)position);
    clothoid.setStartDirection(direction);
    clothoid.setStartCurvature(node->getCurvature());
    glBegin(GL_LINE_STRIP);
    for (int i = -60; i <= 60; ++i) {
        float len = size * i / 10;
        Vec2f pos = clothoid.positionAtLength(len);
        glVertex3f(pos[0], pos[1], position[2]);
    }
    glEnd();

    // Draw handles
    Vec3f handles[] = {
        (clothoid.positionAtLength(size * 6), position[2]),
        (clothoid.positionAtLength(size * -6), position[2]),
        position + 3 * size * directionVector,
        position - 3 * size * directionVector
    };
    glBegin(GL_POINTS);
    for (const Vec3f &handle: handles)
        glVertex3fv((const float *)handle);
    glEnd();
}

void TrackMode::renderGL(RendererOpenGL *renderer)
{
    if (selectedNode) {
        glLineWidth(3);
        glPointSize(10);
        glColor3f(1, 0, 0);
        renderTrackNode(renderer, selectedNode);

        glLineWidth(1);
        glPointSize(8);
        glColor3f(0.5, 0.5, 0.5);
        renderTrackNode(renderer, selectedNode);
    }
    if (hoverNode) {
        glLineWidth(1);
        glPointSize(8);
        glColor3f(1, 1, 1);
        renderTrackNode(renderer, hoverNode);
    }
}
