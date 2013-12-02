#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include <gl/glut.h>

typedef enum {
    NONE   = -1,
    LEFT   = GLUT_LEFT_BUTTON,
    MIDDLE = GLUT_MIDDLE_BUTTON,
    RIGHT  = GLUT_RIGHT_BUTTON
} EMouseButton;

extern int g_currentObj;
extern int g_isCurrentObject;
extern EMouseButton g_mouseCurrentButton;
extern int g_mousePreviousX;
extern int g_mousePreviousY;
extern int g_switchLight;

void callKeyboardFunc(unsigned char key, int x, int y);
void callSpecialFunc(int key, int x, int y);
void callMouseFunc(int button,int state, int x, int y);
void callMotionFunc(int x, int y);

#endif