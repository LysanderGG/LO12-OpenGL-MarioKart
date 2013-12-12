#define _USE_MATH_DEFINES

#include "animations.h"
#include "scene3ds.h"
#include <math.h>

double g_kartAngle = 0.0f;
double g_kartX = 0.0f;
double g_kartY = 0.0f;
double g_kartZ = 0.0f;


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
    Lib3dsMesh * obj;
    int i, j;
    double dx, dy, dz;

    dz = 0.0f;
    dx = _d * cos(g_kartAngle * M_PI / 180.0);
    dy = _d * sin(g_kartAngle * M_PI / 180.0);

    g_kartX += dx;
    g_kartY += dy;
    g_kartZ += dz;

    // Iterate on objects
    for(i = 0; i < g_scenes3DS[KART_ID].lib3dsfile->nmeshes; ++i) {
        obj = g_scenes3DS[KART_ID].lib3dsfile->meshes[i];
        for(j = 0; j < obj->nvertices; ++j) {
            obj->vertices[j][0] += dx;
            obj->vertices[j][1] += dy;
            obj->vertices[j][2] += dz;
        }
    }
}

/*
 * Rotates the kart of an angle _a around the z axis.
 * _a is in degree.
 */
void rotateKart(double _a) {
    g_kartAngle += _a + 360 % 360;
}
