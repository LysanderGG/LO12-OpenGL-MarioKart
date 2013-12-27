#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <3DS/lib3ds.h>
#include "scene.h"

void animationTimer(int value);

void translate(OBJET* _obj, double _dx, double _dy, double _dz);
void rotate(OBJET* _obj, double _ax, double _ay, double _az);

void moveKartForward(double _d);
void moveKart(double _dx, double _dy, double _dz);
void rotateKart(double _a);

#endif