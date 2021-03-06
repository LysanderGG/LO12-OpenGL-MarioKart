#include <stdio.h>
#include <math.h>

#include "affiche.h"
#include "animations.h"
#include "eclairage.h"
#include "interactions.h"
#include "observateur.h"
#include "scene.h"
#include "SceneUtils.h"
#include "skybox.h"

extern int g_haltAnimation;
int g_currentObj        = -1;
int g_current3DSScene   = 0;
int g_isCurrentObject   = 0;
EMouseButton g_mouseCurrentButton = NONE;
int g_mousePreviousX    = 0;
int g_mousePreviousY    = 0;
int g_switchLight       = 1;
int g_dayTime = DAY;
unsigned char g_eventList[EVENT_LIST_MAX_SIZE];
unsigned int  g_eventListSize  = 0;
int           g_eventDirection = 1;
int           g_isShiftDown    = 0;

// debug variables
int g_debugLights = 1;
int g_debugRepere = 1;

void callSpecialFunc(int key, int x, int y) {
    int sens; /* sens positif ou negatif */

    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        sens = -1;
    } else {
        sens = 1;
    }

    // Global modifications
    switch(key) {
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
            if(scene->tabsource[0].allume == 0) {
                scene->tabsource[0].allume = 1;
            } else {
                scene->tabsource[0].allume = 0;
            }
            break;
        case GLUT_KEY_F8: /* switch lighting */
            g_switchLight ^= 1;
            break;
        case GLUT_KEY_F9: /* turn on or off infinite light */
            if(g_dayTime == DAY) {
                g_dayTime = NIGHT;
                charger_skybox(NIGHT);
                scene->tabsource[0].allume = 0;
                turnOnLight(g_scenes3DS[ANIMATED_KART_ID].lights[0]);
                turnOnLight(g_scenes3DS[ANIMATED_KART_ID].lights[1]);
            }
            else if(g_dayTime == NIGHT) {
                g_dayTime = DAY;
                charger_skybox(DAY);
                scene->tabsource[0].allume = 1;
                turnOffLight(g_scenes3DS[ANIMATED_KART_ID].lights[0]);
                turnOffLight(g_scenes3DS[ANIMATED_KART_ID].lights[1]);
            }
            redefineLights();
            break;
        case GLUT_KEY_F10:
            switchLight(g_scenes3DS[ANIMATED_KART_ID].lights[0]);
            switchLight(g_scenes3DS[ANIMATED_KART_ID].lights[1]);
            redefineLights();
            break;
        case GLUT_KEY_F11: /* turn on or off kart's lights */
            switchLight(g_scenes3DS[KART_ID].lights[0]);
            switchLight(g_scenes3DS[KART_ID].lights[1]);
            redefineLights();
            break;
        case GLUT_KEY_F12: /* rotate selon direction axe z */
            rotateZ(sens * OBS_ROTATING_Z);
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
    removeFromEventList(key);
}

void callKeyboardDownFunc(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            /*g_currentObj = ((g_currentObj + getTotalNbObjects() + 1 + g_eventDirection) % (getTotalNbObjects() + 1));
            if(g_currentObj == getTotalNbObjects()) {
                g_currentObj = -1;
            }*/
            addToEventList(key);
            break;
        case 'h':
        case 'H':
            print_help();
            break;
        case 'm':
        case 'M':
            g_haltAnimation ^= 1;
            glutTimerFunc(10, animationTimer, 0);
            break;
        case 'o':
        case 'O':
            changeMode();
            break;
        case 'r':
        case 'R':
            reload_random_objects(MODELE_FILE_NAME);
            break;
        case 'l':
        case 'L':
            g_debugLights ^= 1;
            g_debugRepere ^= 1;
            break;
        case 'z':
        case 'Z':
        case 's':
        case 'S':
        case 'q':
        case 'Q':
        case 'd':
        case 'D':
            addToEventList(key);
            break;
    }
}

void addToEventList(unsigned char key) {
    unsigned int i;

    // Check if the value is already in the event list
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

void removeFromEventList(unsigned char key) {
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

int isInEventList(unsigned char key) {
    unsigned int i;
    for(i = 0; i < g_eventListSize; ++i) {
        if(key == g_eventList[i]) {
            return 1;
        }
    }
    return 0;
}

void handleKeyboardEvents() {
    unsigned int i;
    unsigned char key;
    float speedModificator = 1.0f;

    if(isInEventList(' ')) {
        speedModificator = 5.0f;
    }

    // Parse event list
    for(i = 0; i < g_eventListSize; ++i) {
        key = g_eventList[i];

        switch (key) {
            case 'z':
            case 'Z':
                moveKartForward(speedModificator * TRANSLATING);
                break;
            case 's':
            case 'S':
                moveKartForward(- speedModificator * TRANSLATING);
                break;
            case 'q':
            case 'Q':
                rotateKart(ROTATING);
                break;
            case 'd':
            case 'D':
                rotateKart(-ROTATING);
                break;
            default:
                break;
        }
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
              "### Sources lumineuses\n"
              "F9   : allumer/eteindre la source a l'infini\n"
              "F10  : allumer/eteindre le kart anime\n"
              "F11  : allumer/eteindre le kart bougeable\n"
              "### Observateur\n"
              "F12  : l'observateur tourne autour de l'axe z de la scene\n"
              "LEFT : l'observateur tourne la tete a droite\n"
              "RIGHT: l'observateur tourne la tete a gauche\n"
              "UP   : l'observateur leve la tete\n"
              "DOWN : l'observateur baisse la tete\n"
              "### Kart\n"
              "Z/S  : avancer/reculer\n"
              "Q/D  : tourner vers la gauche/droite\n"
              "SPACE: avancer/reculer plus vite\n"
              "### Modificateurs de scene\n"
              "L    : afficher/cacher les dessins de debug\n"
              "M    : demarrer/arreter l'animation\n"
              "O    : Changer le mode de l'observateur\n"
              "R    : Nouveau calcul des objets aleatoires\n"
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