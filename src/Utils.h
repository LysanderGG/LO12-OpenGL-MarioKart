#ifndef UTILS_H
#define UTILS_H

#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

typedef float Lib3dsMatrix[4][4];

void mat3dsToOpenGL(Lib3dsMatrix mat3ds, float* matGL);

#endif