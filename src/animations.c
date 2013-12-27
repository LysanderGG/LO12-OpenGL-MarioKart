#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "affiche.h"
#include "animations.h"
#include "observateur.h"
#include "scene3ds.h"
#include "Utils.h"

int g_currentFrame = 0;
int g_haltAnimation = 0;

void animationTimer(int value) {
    GLfloat* matGL;
    double angle;

    if (g_haltAnimation != 0) {
        printf("current frame pgm: %d, cru fr 3ds: %d\n", g_currentFrame, g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->current_frame);

        //est cense charger la nouvelle matrice de transfo du pas suivant
        lib3ds_file_eval(g_scenes3DS[ANIMATED_KART_ID].lib3dsfile, g_currentFrame);
        g_currentFrame = (g_currentFrame + 1) % g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->frames;

        matGL = (GLfloat*)malloc(16*sizeof(GLfloat));
        mat3dsToOpenGL(g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix, matGL);
        g_scenes3DS[ANIMATED_KART_ID].translate[0] += matGL[12];
        g_scenes3DS[ANIMATED_KART_ID].translate[1] += matGL[13];
        g_scenes3DS[ANIMATED_KART_ID].translate[2] += matGL[14];

        /*printf("matrice GL 0:%f, 1:%f, 2:%f, 3:%f\n\
                4:%f, 5:%f, 6:%f, 7:%f\n\
                8:%f, 9:%f, 10:%f, 11:%f\n\
                12:%f, 13:%f, 14:%f, 15:%f\n\n",
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[0][0],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[1][0],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[2][0],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[3][0],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[0][1],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[1][1],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[2][1],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[3][1],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[0][2],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[1][2],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[2][2],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[3][2],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[0][3],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[1][3],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[2][3],
                g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->meshes[0]->matrix[3][3]
            );*/

        angle = acos(matGL[0]);
        moveKart(matGL[12], matGL[13], matGL[14]);
        rotateKart(angle);

        glutTimerFunc(10, animationTimer, 0);
    }
}

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

void moveKart(double _dx, double _dy, double _dz) {

    g_scenes3DS[ANIMATED_KART_ID].translate[0] += _dx;
    g_scenes3DS[ANIMATED_KART_ID].translate[1] += _dy;
    g_scenes3DS[ANIMATED_KART_ID].translate[2] += _dz;

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
