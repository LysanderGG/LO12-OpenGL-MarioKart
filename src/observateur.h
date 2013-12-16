#ifndef OBSERVATEUR_H
#define OBSERVATEUR_H

#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

// Defines
#define OBS_MODE_POLAR_VIEW        0
#define OBS_MODE_PILOT_VIEW        1
#define OBS_MODE_KART_VIEW         2
#define NB_OBS_MODES               3

#define DEFAULT_OBS_MODE        OBS_MODE_POLAR_VIEW

#define DEFAULT_POLAR_DISTANCE      2.5
#define DEFAULT_POLAR_AZIMUT        -30
#define DEFAULT_POLAR_ELEVATION     20
#define DEFAULT_POLAR_TWIST          0

#define DEFAULT_PLANE_X             5
#define DEFAULT_PLANE_Y             0
#define DEFAULT_PLANE_Z             0
#define DEFAULT_PLANE_ROULIS        90
#define DEFAULT_PLANE_TANGAGE       90
#define DEFAULT_PLANE_LACET         180

#define KART_VIEW_X_OFFSET 0
#define KART_VIEW_Y_OFFSET 0
#define KART_VIEW_Z_OFFSET 0

// Variables globales
extern char*  g_obsModeTitles[3];

extern int    g_obsMode;
extern double g_polarDistance;
extern double g_polarAzimut;
extern double g_polarElevation;
extern double g_polarTwist;

extern double g_planeX;
extern double g_planeY;
extern double g_planeZ;
extern double g_planeRoulis;
extern double g_planeTangage;
extern double g_planeLacet;

extern double g_kartRoulis;
extern double g_kartTangage;
extern double g_kartLacet;

void initView       ();
void changeMode     ();
void recomputeView  ();
void polarView      (double distance, double azimut, double elevation, double twist);
void pilotView      (double planeX, double planeY, double planeZ, double roulis, double tangage, double lacet);
void rotateZ        (double _az);
void rotateHead     (double _az);
void nodHead        (double _az);
void zoom           (double _d);
void translateX     (double _x);
void translateY     (double _y);

double getPlaneDistance();

#endif
