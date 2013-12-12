#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "scene.h"

#define KART_ID 1

extern double g_kartAngle;

void translate(OBJET* _obj, double _dx, double _dy, double _dz);
void rotate(OBJET* _obj, double _ax, double _ay, double _az);

void moveKartForward(double _d);

#endif