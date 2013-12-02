#include <stdio.h>
#include <math.h>

#include "interactions.h"
#include "animations.h"
#include "scene.h"
#include "observateur.h"

#define TRANSLATING			1
#define ROTATING			10
#define OBS_ROTATING_Z		5
#define OBS_ROTATING_HEAD	5
#define OBS_NODING_HEAD		5

int g_currentObj = -1;
int g_isCurrentObject = 0;
EMouseButton g_mouseCurrentButton = NONE;
int g_mousePreviousX = 0;
int g_mousePreviousY = 0;
int g_switchLight = 1;

void callSpecialFunc(int key, int x, int y) {
	int sens; /* sens positif ou négatif */

	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		sens = -1;
    } else {
		sens = 1;
    }

	// Global modifications
	switch(key) {
		case GLUT_KEY_F10: /* rotate selon direction axe z */
			rotateZ(sens * OBS_ROTATING_Z);
			break;
		case GLUT_KEY_LEFT:
			rotateHead(OBS_ROTATING_HEAD);
			break;
		case GLUT_KEY_RIGHT:
			rotateHead(-OBS_ROTATING_HEAD);
			break;
		case GLUT_KEY_UP:
			nodHead(-OBS_NODING_HEAD);
			break;
		case GLUT_KEY_DOWN:
			nodHead(OBS_NODING_HEAD);
			break;
        case GLUT_KEY_F8: /* switch lighting */
            g_switchLight ^= 1;
            break;
	}

	// Is an object currently selected ?
	if(g_currentObj < 0) { return ; }

	// Local modification ==> On an object
	switch (key) {
		case GLUT_KEY_F1: /* avance selon direction axe x */
			translate(&(scene->tabobj[g_currentObj]), TRANSLATING * sens, 0.0, 0.0);
			break;
		case GLUT_KEY_F2: /* avance selon direction axe y */
			if(g_currentObj < 0) { return ; }
			translate(&(scene->tabobj[g_currentObj]), 0.0, TRANSLATING * sens, 0.0);
			break;
		case GLUT_KEY_F3: /* avance selon direction axe z */
			if(g_currentObj < 0) { return ; }
			translate(&(scene->tabobj[g_currentObj]), 0.0, 0.0, TRANSLATING * sens);
			break;
		case GLUT_KEY_F4: /* rotate selon direction axe x */
			if(g_currentObj < 0) { return ; }
			rotate(&(scene->tabobj[g_currentObj]), ROTATING * sens, 0.0, 0.0);
			break;
		case GLUT_KEY_F5: /* rotate selon direction axe y */
			if(g_currentObj < 0) { return ; }
			rotate(&(scene->tabobj[g_currentObj]), 0.0, ROTATING * sens, 0.0);
			break;
		case GLUT_KEY_F6: /* rotate selon direction axe z */
			if(g_currentObj < 0) { return ; }
			rotate(&(scene->tabobj[g_currentObj]), 0.0, 0.0, ROTATING * sens);
			break;
	}

	glutPostRedisplay();
}

void callKeyboardFunc(unsigned char key, int x, int y){
	int sens; /* sens positif ou négatif */
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		sens = -1;
    } else {
		sens = 1;
    }

	switch (key) {
		case ' ':
			g_currentObj = abs((g_currentObj + sens) % (scene->nbobj + 1));
            if(g_currentObj == scene->nbobj) {
                g_currentObj = -1;
            }
			break;
		case 'o':
		case 'O':
			changeMode();
			break;
	}

	glutPostRedisplay();
}

void callMouseFunc(int button,int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_DOWN && g_mouseCurrentButton == NONE) {
            g_mouseCurrentButton = (EMouseButton)button;
            g_mousePreviousX = x;
            g_mousePreviousY = y;
        } else if(g_mouseCurrentButton == button) {
            g_mouseCurrentButton = NONE;
            g_mousePreviousX = 0;
            g_mousePreviousY = 0;
        }
    }
}

void callMotionFunc(int x, int y) {
    switch(g_mouseCurrentButton) {
    case LEFT :
        rotateHead  ((g_mousePreviousX - x) / 10.0);
        nodHead     ((g_mousePreviousY - y) / 10.0);
        break;
    case MIDDLE :
        rotateZ     ((g_mousePreviousX - x) / 10.0);
        zoom        ((g_mousePreviousY - y) / 10.0);
        break;
    case RIGHT :
        translateY  ((g_mousePreviousX - x) / 10.0);
        translateX  ((g_mousePreviousY - y) / 10.0);
        break;
    default:
        break;
    }

    g_mousePreviousX = x;
    g_mousePreviousY = y;
}
