#ifndef UTILS_H
#define UTILS_H

#include "scene.h"

//#define max(a,b) a > b ? a : b
//#define min(a,b) a < b ? a : b

typedef float Lib3dsMatrix[4][4];

void calcule_normales(SCENE *scene);
void calcule_vecteur(MCOORD p1, MCOORD p2, MCOORD *v);
int prod_scalaire(MCOORD u, MCOORD v);
void prod_vectoriel(MCOORD u, MCOORD v, MCOORD *w);
int isInTriangle(MCOORD A, MCOORD B, MCOORD C, MCOORD M);
void normalise(MCOORD *v);

void getMatrix4x4Row(float* matrix4x4, float* out_row, int lineIdx);
void matrix4x4Product(float* a, float* b, float* res);
void mat3dsToOpenGL(Lib3dsMatrix mat3ds, GLdouble* matGL);
char* concat(char* s1, char* s2);

#endif
