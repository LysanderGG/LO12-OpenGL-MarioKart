#include <stdlib.h>

#include "Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>



/* -----------------------------------------------
Cette fonction realise la norme euclidienne d'un
vecteur v.
On ne prend pas en compte la quatrieme composante.
----------------------------------------------- */

GLdouble norme(MCOORD v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/* -----------------------------------------------
Cette fonction realise le produit vectoriel de deux
vecteurs
  w = u ^ v
----------------------------------------------- */

void prod_vectoriel(MCOORD u, MCOORD v, MCOORD *w) {
    w->x = u.y * v.z - u.z * v.y;
    w->y = u.x * v.z - u.z * v.x;
    w->z = u.x * v.y - u.y * v.x;
}

void calcule_vecteur(MCOORD p1, MCOORD p2, MCOORD *v) {
    v->x = p2.x - p1.x;
    v->y = p2.y - p1.y;
    v->z = p2.z - p1.z;
}

void normalise(MCOORD *v) {
    GLdouble norm = norme(*v);
    if(norm > 0.0000001) {
        v->x /= norm;
        v->y /= norm;
        v->z /= norm;
    } else {
        v->x = 0.0;
        v->y = 0.0;
        v->z = 0.0;
    }
}

// Matrix 4x4 format
//
// m0  m4  m8  m12
// m1  m5  m9  m13
// m2  m6  m10 m14 
// m3  m7  m11 m15
void getMatrix4x4Row(float* matrix4x4, float* out_row, int lineIdx) {
    if(lineIdx < 0 || lineIdx > 3) {
        return;
    }

    out_row[0] = matrix4x4[0  + lineIdx];
    out_row[1] = matrix4x4[4  + lineIdx];
    out_row[2] = matrix4x4[8  + lineIdx];
    out_row[3] = matrix4x4[12 + lineIdx];
}

// Matrix 4x4 format
//
// m0  m4  m8  m12
// m1  m5  m9  m13
// m2  m6  m10 m14 
// m3  m7  m11 m15
void matrix4x4Product(float* a, float* b, float* res) {
    int i;

    for(i = 0; i < 16; ++i) {
        res[i] = a[0  + i%4] * b[0 + (int)floor(i/4.0) * 4] 
               + a[4  + i%4] * b[1 + (int)floor(i/4.0) * 4]
               + a[8  + i%4] * b[2 + (int)floor(i/4.0) * 4]
               + a[12 + i%4] * b[3 + (int)floor(i/4.0) * 4];
    }
}


void mat3dsToOpenGL(Lib3dsMatrix mat3ds, GLdouble* matGL) {
    matGL[0]  = mat3ds[0][0];
    matGL[4]  = mat3ds[1][0];
    matGL[8]  = mat3ds[2][0];
    matGL[12] = mat3ds[3][0];
    matGL[1]  = mat3ds[0][1];
    matGL[5]  = mat3ds[1][1];
    matGL[9]  = mat3ds[2][1];
    matGL[13] = mat3ds[3][1];
    matGL[2]  = mat3ds[0][2];
    matGL[6]  = mat3ds[1][2];
    matGL[10] = mat3ds[2][2];
    matGL[14] = mat3ds[3][2];
    matGL[3]  = mat3ds[0][3];
    matGL[7]  = mat3ds[1][3];
    matGL[11] = mat3ds[2][3];
    matGL[15] = mat3ds[3][3];
}

char* concat(char* s1, char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    char* result = (char*)malloc((len1+len2+1)*sizeof(char)); //+1 for the zero-terminator

    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1); //+1 to copy the null-terminator

    return result;
}
