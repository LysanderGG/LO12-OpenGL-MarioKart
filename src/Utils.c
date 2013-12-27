#include "Utils.h"

void mat3dsToOpenGL(Lib3dsMatrix mat3ds, float* matGL) {
    matGL[0] = mat3ds[0][0];
    matGL[1] = mat3ds[1][0];
    matGL[2] = mat3ds[2][0];
    matGL[3] = mat3ds[3][0];
    matGL[4] = mat3ds[0][1];
    matGL[5] = mat3ds[1][1];
    matGL[6] = mat3ds[2][1];
    matGL[7] = mat3ds[3][1];
    matGL[8] = mat3ds[0][2];
    matGL[9] = mat3ds[1][2];
    matGL[10] = mat3ds[2][2];
    matGL[11] = mat3ds[3][2];
    matGL[12] = mat3ds[0][3];
    matGL[13] = mat3ds[1][3];
    matGL[14] = mat3ds[2][3];
    matGL[15] = mat3ds[3][3];
}
