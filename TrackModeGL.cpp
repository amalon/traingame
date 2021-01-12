#include "TrackMode.h"
#include "TrackNode.h"
#include "TrackPosition.h"
#include "RendererOpenGL.h"
#include "Clothoid.h"

#include <GL/gl.h>

static void renderTrackNode(const Renderer *renderer, const TrackNode *node)
{
    float direction = node->getDirection();
    float size = renderer->getViewpoint().viewportToWorldSize(node->getPosition(), 0.02);

    // Draw a circle around the node at each track
    for (int i = 0; i < node->getNumTracks(); ++i) {
        glBegin(GL_LINE_LOOP);
        const Vec3f &position = node->getPosition(i);
        for (int i = 0; i < 32; ++i) {
            float theta = M_PI * 2 * i / 32;
            float sin, cos;
            sincosf(theta, &sin, &cos);
            glVertex3f(position[0] + size * sin,
                        position[1] + size * cos,
                        position[2]);
        }
        glEnd();

        // Draw a curve showing the curvature through the node
        Clothoid<float, float> clothoid;
        clothoid.setStartPosition((Vec2f)position);
        clothoid.setStartDirection(direction);
        clothoid.setStartCurvature(node->getCurvature(i));
        glBegin(GL_LINE_STRIP);
        for (int i = -60; i <= 60; ++i) {
            float len = size * i / 10;
            Vec2f pos = clothoid.positionAtLength(len);
            glVertex3f(pos[0], pos[1], position[2]);
        }
        glEnd();

        // Draw arrow showing point directions
        TrackPosition pos;
        pos.set(node, true, i);

        if (pos) {
            for (int d = -1; d <= 1; d += 2) {
                if (!node->hasPoints(i, d > 0))
                    continue;
                glBegin(GL_LINE_STRIP);
                glVertex3fv((const float *)position);
                for (int j = 1; j <= 30; ++j) {
                    TrackPosition pos2 = pos + size*j*d/3;
                    if (pos2)
                        glVertex3fv((const float *)pos2.getPosition());
                }
                glEnd();
            }
        }
    }

    // Draw a line perpendicular to the direction across the node
    Vec3f position1 = node->getPosition(0);
    Vec3f position2 = node->getPosition(node->getNumTracks() - 1);
    Vec3f directionVector;
    sincosf(direction + M_PI/2, &directionVector[1], &directionVector[0]);
    directionVector[2] = 0;
    glBegin(GL_LINES);
    glVertex3fv((const float *)(position1 + 3 * size * directionVector));
    glVertex3fv((const float *)(position2 - 3 * size * directionVector));
    glEnd();
}

void TrackMode::renderHandlesGL(RendererOpenGL * renderer)
{
    // Draw handles
    glBegin(GL_POINTS);
    for (const Handle &handle: handles)
        if (handle.enabled)
            glVertex3fv((const float *)handle.position);
    glEnd();
}

void TrackMode::renderGL(RendererOpenGL *renderer)
{
    if (selectedNode) {
        glLineWidth(3);
        glPointSize(10);
        glColor3f(1, 0, 0);
        renderTrackNode(renderer, selectedNode);
        renderHandlesGL(renderer);

        glLineWidth(1);
        glPointSize(8);
        glColor3f(0.5, 0.5, 0.5);
        renderTrackNode(renderer, selectedNode);
        renderHandlesGL(renderer);
    }
    if (hoverNode) {
        glLineWidth(1);
        glPointSize(8);
        glColor3f(1, 1, 1);
        renderTrackNode(renderer, hoverNode);
    }

    if (*testPos) {
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3f(0, 0, 1);
        glVertex3fv((const float *)testPos->getPosition());
        glEnd();
    }
}
