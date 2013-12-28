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
    Lib3dsNode* node;
    //FILE *f;

    if (g_haltAnimation != 0) {
        //printf("animation frame %d\n", g_currentFrame);
        lib3ds_file_eval(g_scenes3DS[ANIMATED_KART_ID].lib3dsfile, g_currentFrame);
        g_currentFrame = (g_currentFrame + 1) % g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->frames;

        if(g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->nodes->name != "$$$DUMMY")
            node = g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->nodes->next;
        else
            node = g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->nodes;

        g_scenes3DS[ANIMATED_KART_ID].translateAnimation[0] = node->matrix[3][0];
        g_scenes3DS[ANIMATED_KART_ID].translateAnimation[1] = node->matrix[3][1];
        g_scenes3DS[ANIMATED_KART_ID].translateAnimation[2] = node->matrix[3][2];

        /*if(g_currentFrame == 10) {
            f = fopen("animation10.txt", "w");
            if (f == NULL) {
                printf("Error opening file!\n");
                return;
            }
            fprintf(f, "frame %d\n", g_currentFrame);
            writeNodeMatrix(f, g_scenes3DS[ANIMATED_KART_ID].lib3dsfile->nodes);
            fclose(f);
        }*/

        glutTimerFunc(10, animationTimer, 0);
    }
}

/**
* write into the file f the transformation matrix
* of the node root
*/
void writeNodeMatrix(FILE* f, Lib3dsNode* root) {
    fprintf(f, "node %s \n", root->name);
    fprintf(f, "0:%f, 1:%f, 2:%f, 3:%f\n4:%f, 5:%f, 6:%f, 7:%f\n8:%f, 9:%f, 10:%f, 11:%f\n12:%f, 13:%f, 14:%f, 15:%f\n\n",
            root->matrix[0][0],
            root->matrix[1][0],
            root->matrix[2][0],
            root->matrix[3][0],
            root->matrix[0][1],
            root->matrix[1][1],
            root->matrix[2][1],
            root->matrix[3][1],
            root->matrix[0][2],
            root->matrix[1][2],
            root->matrix[2][2],
            root->matrix[3][2],
            root->matrix[0][3],
            root->matrix[1][3],
            root->matrix[2][3],
            root->matrix[3][3]
        );
    if(root->next != 0)
        writeNodeMatrix(f, root->next);
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
