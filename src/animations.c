#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "affiche.h"
#include "animations.h"
#include "observateur.h"
#include "scene3ds.h"


void translate(OBJET* _obj, double _dx, double _dy, double _dz) {
    _obj->transfo[12] += _dx;
    _obj->transfo[13] += _dy;
    _obj->transfo[14] += _dz;
}

void rotate(OBJET* _obj, double _ax, double _ay, double _az) {
    glPushMatrix();
    glLoadMatrixd(_obj->transfo);
    glRotated(_ax, 1.0, 0.0, 0.0);
    glRotated(_ay, 0.0, 1.0, 0.0);
    glRotated(_az, 0.0, 0.0, 1.0);
    glGetDoublev(GL_MODELVIEW_MATRIX, _obj->transfo);
    glPopMatrix();
}

/*
 * Moves the kart forward of a distance _d.
 * If _d is < 0, the kart will move backward.
 */
void moveKartForward(double _d) {
    double dx, dy, dz;

    dx = _d * cos(g_scenes3DS[KART_ID].rotate[2] * M_PI / 180.0);
    dy = _d * sin(g_scenes3DS[KART_ID].rotate[2] * M_PI / 180.0);
    dz = 0.0f;

    g_scenes3DS[KART_ID].translate[0] += dx;
    g_scenes3DS[KART_ID].translate[1] += dy;
    g_scenes3DS[KART_ID].translate[2] += dz;

    recomputeView();
}

/*
 * Rotates the kart of an angle _a around the z axis.
 * _a is in degree.
 */
void rotateKart(double _a) {
    g_scenes3DS[KART_ID].rotate[2] = fmod(g_scenes3DS[KART_ID].rotate[2] + _a + 360, 360);

    recomputeView();
}
