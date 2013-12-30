#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include <gl/glut.h>

typedef enum {
    NONE   = -1,
    LEFT   = GLUT_LEFT_BUTTON,
    MIDDLE = GLUT_MIDDLE_BUTTON,
    RIGHT  = GLUT_RIGHT_BUTTON
} EMouseButton;

#define EVENT_LIST_MAX_SIZE 10
#define TRANSLATING         1
#define ROTATING            5
#define OBS_ROTATING_Z      5
#define OBS_ROTATING_HEAD   5
#define OBS_NODING_HEAD     5

// ----------------------------------------------
// Variables
extern int g_currentObj;
extern int g_current3DSScene;
extern int g_isCurrentObject;
extern EMouseButton g_mouseCurrentButton;
extern int g_mousePreviousX;
extern int g_mousePreviousY;
extern int g_switchLight;

// debug variables
extern int g_debugLights;
extern int g_debugRepere;

// ----------------------------------------------
// Functions
void callKeyboardUpFunc(unsigned char key, int x, int y);
void callKeyboardDownFunc(unsigned char key, int x, int y);
void handleKeyboardEvents();
void callSpecialFunc(int key, int x, int y);
void callMouseFunc(int button,int state, int x, int y);
void callMotionFunc(int x, int y);

void addToEventList(unsigned char key);
void removeFromEventList(unsigned char key);

void print_help();

#endif