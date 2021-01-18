#include "TrainBogie.h"
#include "TrainWheelset.h"
#include "TrackPosition.h"
#include "RendererOpenGL.h"

#include <GL/gl.h>

void TrainBogie::renderGL(RendererOpenGL *renderer)
{
    for (BogieConnection &bogie: bogies)
        renderer->renderObj(bogie.bogie);

    for (TrainWheelset *wheelset: wheelsets)
        renderer->renderObj(wheelset);

    Vec3f forward, side;
    sincosf(direction, &forward[1], &forward[0]);
    side[0] = -forward[1];
    side[1] = forward[0];
    forward[2] = side[2] = 0;

    glLineWidth(5);
    glColor3f(0, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv((const float*)(position - backwardExtent*forward - side * (width/2)));
    glVertex3fv((const float*)(position - backwardExtent*forward + side * (width/2)));
    glVertex3fv((const float*)(position + forwardExtent*forward + side * (width/2)));
    glVertex3fv((const float*)(position + forwardExtent*forward - side * (width/2)));
    glEnd();
}
