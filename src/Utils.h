#ifndef UTILS_H
#define UTILS_H

#include "scene.h"

#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

typedef float Lib3dsMatrix[4][4];


void calcule_normales(SCENE *scene);
void mat3dsToOpenGL(Lib3dsMatrix mat3ds, float* matGL);
void getMatrix4x4Row(float* matrix4x4, float* out_row, int lineIdx);
void matrix4x4Product(float* a, float* b, float* res);

#endif
