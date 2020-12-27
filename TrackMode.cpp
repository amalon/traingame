#include "TrackMode.h"
#include "Renderer.h"
#include "Clothoid.h"

#include <GL/gl.h>

#include <cmath>

TrackMode::TrackMode(Railway *newRailway)
: railway(newRailway),
  step(0),
  start(0.0f),
  end(0.0f),
  hoverNode(nullptr),
  selectedNode(nullptr),
  dragMode(NONE)
{
    mouseRay.start.set(0);
    mouseRay.norm.set(0);
}

void TrackMode::mouseMove(const LineNormal3f &ray)
{
    float range = renderer->getViewpoint().viewportToWorldSize(ray.startPosition(), 0.02);
    TrackNode *closestNode = railway->findClosestNode(ray, range);
    if (hoverNode != closestNode) {
        hoverNode = closestNode;
        renderer->setRedraw();
    }

    switch (step) {
    case 1:
        end = ray.start;
        renderer->setRedraw();
        break;
    case 2:
        switch (dragMode) {
        case MOVE:
            if (!mouseRay.norm.zero()) {
                Vec3f diff = ray.start - mouseRay.start;
                selectedNode->setPosition(selectedNode->getPosition() + diff);
                renderer->setRedraw();
            }
            break;
        default:
            break;
        }
    }

    mouseRay = ray;
}

void TrackMode::mouseLeave()
{
    mouseRay.norm.set(0);

    switch (step) {
    case 1:
        end = start;
        renderer->setRedraw();
        break;
    }
}

void TrackMode::mouseDown(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        switch (step) {
        case 0:
            step = 1;
            start = ray.start;
            end = start;
            renderer->setRedraw();
            break;
        case 1: {
            if (start == ray.start)
                break;

            step = 2;
            end = ray.start;

            Vec2f vec(end - start);
            float direction = atan2f(vec[1], vec[0]);

            TrackNode *startNode = new TrackNode();
            startNode->setPosition(start);
            startNode->setDirection(direction);
            railway->addNode(startNode);

            TrackNode *endNode = new TrackNode();
            endNode->setPosition(end);
            endNode->setDirection(direction);
            railway->addNode(endNode);

            TrackSection *section = new TrackSection(startNode->forward(),
                                                     endNode->backward());
            railway->addSection(section);

            renderer->setRedraw();
            break;
        }
        case 2:
            if (selectedNode != hoverNode)
                renderer->setRedraw();
            selectedNode = hoverNode;
            if (selectedNode)
                dragMode = MOVE;
            break;
        }
    }
}

void TrackMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
    // Left button
    if (button == 0) {
        switch (step) {
        case 2:
            // Finish dragging
            dragMode = NONE;
            break;
        }
    }
}

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

void TrackMode::renderUI()
{
    if (step < 1)
        return;

    glLineWidth(2);
    glPointSize(4);

    if (step == 1) {
        glBegin(GL_LINE_STRIP);
        glColor3f(0, 0, 0);
        glVertex3fv((const float *)start);
        glVertex3fv((const float *)end);
        glEnd();

        glBegin(GL_POINTS);
        glColor3f(0.3f, 0.3f, 0.3f);
        glVertex3fv((const float *)start);
        glVertex3fv((const float *)end);
        glEnd();
    }

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
