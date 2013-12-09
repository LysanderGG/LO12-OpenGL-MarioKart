#include <stdio.h>
#include <math.h>

#include "interactions.h"
#include "animations.h"
#include "scene.h"
#include "observateur.h"

#define TRANSLATING         1
#define ROTATING            10
#define OBS_ROTATING_Z      5
#define OBS_ROTATING_HEAD   5
#define OBS_NODING_HEAD     5

int g_currentObj = -1;
int g_current3DSScene = 0;
int g_isCurrentObject = 0;
EMouseButton g_mouseCurrentButton = NONE;
int g_mousePreviousX = 0;
int g_mousePreviousY = 0;
int g_switchLight = 1;

void callSpecialFunc(int key, int x, int y) {
    int sens; /* sens positif ou negatif */

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
            g_currentObj = ((g_currentObj + getTotalNbObjects() + 1 + sens) % (getTotalNbObjects() + 1));
            if(g_currentObj == getTotalNbObjects()) {
                g_currentObj = -1;
            }
            break;
        case 'o':
        case 'O':
            changeMode();
            break;
        case 'h':
        case 'H':
            print_help();
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

void print_help() {
    char* s = "***************************\n"
              "*** Touches de controle ***\n"
              "***************************\n"
              "F1: avance l'objet selon direction axe x\n"
              "F2: avance l'objet selon direction axe y\n"
              "F3: avance l'objet selon direction axe z\n"
              "F4: tourne l'objet selon axe x\n"
              "F5: tourne l'objet selon axe y\n"
              "F6: tourne l'objet selon axe z\n"
              "F9: allumer/eteindre la source a l'infini\n"
              "F10:   l'observateur tourne autour de l'axe z de la scene\n"
              "LEFT:  l'observateur tourne la tete a droite\n"
              "RIGHT: l'observateur tourne la tete a gauche\n"
              "UP:    l'observateur leve la tete\n"
              "DOWN:  l'observateur baisse la tete\n"
              "*****************************\n"
              "*** Commandes a la souris ***\n"
              "*****************************\n"
              "clic gauche: \n"
              "\t * haut-bas: lever, baisser la tete\n"
              "\t * gauche-droite: tourner la tete a gauche, a droite\n"
              "clic droit: \n"
              "\t * haut-bas: monter-descendre parallelement a l'axe x\n"
              "\t * gauche-droite: aller a gauche, a droite, parallelement a l'axe y"
              "clic milieu: \n"
              "\t * haut-bas: se rapprocher, s'eloigner (effet de zoom)\n"
              "\t * gauche-droite: tourner autour de l'axe z de la scene\n"
              "*****************************\n"
              ;
    
    printf(s);
}