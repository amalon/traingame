#include "TrainWheelset.h"

#include <GL/gl.h>

void TrainWheelset::renderGL(RendererOpenGL *renderer)
{
    Mat22f rotMatrix;
    Vec3f pos = trackPosition.getPosition(&rotMatrix);

    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(1, 0, 1);
    // Axle
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(0, wheelGauge/2))));
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(0, -wheelGauge/2))));
    // Left wheel
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(wheelRadius, wheelGauge/2))));
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(-wheelRadius, wheelGauge/2))));
    // Right wheel
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(wheelRadius, -wheelGauge/2))));
    glVertex3fv((const float *)(pos + (Vec3f)(rotMatrix*Vec2f(-wheelRadius, -wheelGauge/2))));
    glEnd();
}
