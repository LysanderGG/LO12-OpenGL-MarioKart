#include "animations.h"

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
