#include "TrackMode.h"
#include "Renderer.h"

#include <GL/gl.h>

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
        break;
    }
}

void TrackMode::mouseLeave()
{
    switch (step) {
    case 1:
        end = start;
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
            break;
        case 1:
            step = 2;
            end = ray.start;
            break;
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
