#ifndef UTILS_H
#define UTILS_H

#include "scene.h"

//#define max(a,b) a > b ? a : b
//#define min(a,b) a < b ? a : b


void calcule_normales(SCENE *scene);
void calcule_vecteur(MCOORD p1, MCOORD p2, MCOORD *v);
void prod_vectoriel(MCOORD u, MCOORD v, MCOORD *w);
void normalise(MCOORD *v);

void getMatrix4x4Row(float* matrix4x4, float* out_row, int lineIdx);
void matrix4x4Product(float* a, float* b, float* res);
char* concat(char* s1, char* s2);

#endif
