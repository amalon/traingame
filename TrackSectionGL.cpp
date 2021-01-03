#include "TrackSection.h"

#include <GL/gl.h>

void TrackSection::renderGL(RendererOpenGL *renderer)
{
    float maxCurve = 1.0f / 30.0f;
    for (const ClothoidT &clothoid: chain.clothoids()) {
        float length = clothoid.getLength();
        if (!length)
            continue;
        int samples;
        float lengthOffset, lengthDelta;

        samples = (unsigned int)(length/0.65f + 0.5f);
        lengthDelta = length / samples;
        lengthOffset = lengthDelta / 2;

        glBegin(GL_LINES);
        for (int i = 0; i < samples; ++i) {
            float len = lengthOffset + lengthDelta * i;
            float curvature = fabs(clothoid.curvatureAtLength(len));
            float colour = curvature / maxCurve;
            if (colour > 1)
                colour = 1;
            // White to yellow to red
            float red = 1;
            float green = 1;
            float blue = 0;
            if (colour > 0.5f)
                green = (1.0f - colour) * 2;
            else
                blue = (0.5f - colour) * 2;

            glColor3f(red, green, blue);

            float direction = clothoid.directionAtLength(len);
            Vec2f forwardVec;
            sincosf(direction, &forwardVec[1], &forwardVec[0]);
            Vec2f leftVec(-forwardVec[1], forwardVec[0]);

            Vec2f pos = clothoid.positionAtLength(len);
            glVertex2fv((const float *)(pos - leftVec));
            glVertex2fv((const float *)(pos + leftVec));
        }
        glEnd();

        samples = 1 + (unsigned int)length;
        if (samples < 2)
            samples = 2;
        lengthDelta = length / (samples - 1);

        for (int rail = 0; rail < 2; ++rail) {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < samples; ++i) {
                float len = lengthDelta * i;
                float curvature = fabs(clothoid.curvatureAtLength(len));
                float colour = curvature / maxCurve;
                if (colour > 1)
                    colour = 1;
                // White to yellow to red
                float red = 1;
                float green = 1;
                float blue = 0;
                if (colour > 0.5f)
                    green = (1.0f - colour) * 2;
                else
                    blue = (0.5f - colour) * 2;

                glColor3f(red, green, blue);

                float direction = clothoid.directionAtLength(len);
                Vec2f forwardVec;
                sincosf(direction, &forwardVec[1], &forwardVec[0]);
                Vec2f leftVec(-forwardVec[1], forwardVec[0]);

                Vec2f pos = clothoid.positionAtLength(len);
                glVertex2fv((const float *)(pos + leftVec*(-0.7175+1.435*rail)));
            }
            glEnd();
        }
    }
    glColor3f(0, 0, 0);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (const ClothoidT &clothoid: chain.clothoids()) {
        glVertex2fv((const float *)clothoid.getStartPosition());
    }
    glEnd();
}
