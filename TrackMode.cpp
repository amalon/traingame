#include "TrackMode.h"
#include "Renderer.h"

#include <GL/gl.h>

#include <cmath>

TrackMode::TrackMode(Railway *newRailway)
: railway(newRailway),
  step(0),
  start(0.0f),
  end(0.0f)
{
}

void TrackMode::mouseMove(const LineNormal3f &ray)
{
    switch (step) {
    case 1:
        end = ray.start;
        renderer->setRedraw();
        break;
    }
}

void TrackMode::mouseLeave()
{
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
        }
    }
}

void TrackMode::mouseUp(const LineNormal3f &ray, int button, int clicks)
{
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
}
