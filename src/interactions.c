#include <stdio.h>
#include <math.h>

#include "animations.h"
#include "eclairage.h"
#include "interactions.h"
#include "observateur.h"
#include "scene.h"
#include "skybox.h"

int g_currentObj = -1;
int g_current3DSScene = 0;
int g_isCurrentObject = 0;
EMouseButton g_mouseCurrentButton = NONE;
int g_mousePreviousX = 0;
int g_mousePreviousY = 0;
int g_switchLight = 1;
int g_dayTime = DAY;
unsigned char g_eventList[EVENT_LIST_MAX_SIZE];
unsigned int  g_eventListSize = 0;
int           g_eventDirection = 1;

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
        case GLUT_KEY_F7:
            switchLight(GL_LIGHT0);
            break;
        case GLUT_KEY_F8: /* switch lighting */
            g_switchLight ^= 1;
            break;
        case GLUT_KEY_F9: /* turn on or off infinite light */
            if(g_dayTime == DAY) {
                g_dayTime = NIGHT;
                charger_skybox(NIGHT);
                turnOffLight(GL_LIGHT0);
            }
            else if(g_dayTime == NIGHT) {
                g_dayTime = DAY;
                charger_skybox(DAY);
                turnOnLight(GL_LIGHT0);
            }
            break;
        case GLUT_KEY_F11: /* turn on or off kart's lights */
            switchLight(g_scenes3DS[KART_ID].lights[0]);
            switchLight(g_scenes3DS[KART_ID].lights[1]);
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

void callKeyboardUpFunc(unsigned char key, int x, int y) {
    //Remove the key from the event list.
    unsigned int i;
    int found = 0;

    for(i = 0; i < g_eventListSize; ++i) {
        if(g_eventList[i] == key) {
            found = 1;
            --g_eventListSize;
            break;
        }
    }

    if(found) {
        while(i < EVENT_LIST_MAX_SIZE) {
            g_eventList[i] = g_eventList[i++];
        }
    }
}

void callKeyboardDownFunc(unsigned char key, int x, int y) {
    // Check if the value is already in the event list
    unsigned int i;

    // Handles shift pressing.
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        g_eventDirection = -1;
    } else {
        g_eventDirection = 1;
    }

    for(i = 0; i < g_eventListSize; ++i) {
        if(g_eventList[i] == key) {
            return;
        }
    }

    // If the event is not in the event list adds it.
    if(g_eventListSize >= EVENT_LIST_MAX_SIZE) {
        printf("[ERROR] The event list can not handle more than %d events.\n", EVENT_LIST_MAX_SIZE);
        return;
    } else {
        g_eventList[g_eventListSize++] = key;
    }
}

void handleKeyboardEvents() {
    unsigned int i;
    unsigned char key;

    //printf("Event List : { ");

    // Parse event list
    for(i = 0; i < g_eventListSize; ++i) {
        key = g_eventList[i];

        //printf("%d ", key);

        switch (key) {
            case ' ':
                g_currentObj = ((g_currentObj + getTotalNbObjects() + 1 + g_eventDirection) % (getTotalNbObjects() + 1));
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
                break;
            case 'z':
            case 'Z':
                moveKartForward(TRANSLATING);
                break;
            case 's':
            case 'S':
                moveKartForward(-TRANSLATING);
                break;
            case 'q':
            case 'Q':
                rotateKart(ROTATING);
                break;
            case 'd':
            case 'D':
                rotateKart(-ROTATING);
                break;
        }
    }

    //printf("} \n");

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