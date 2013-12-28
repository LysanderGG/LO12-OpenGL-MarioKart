#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "GLee/GLee.h" //GL header file, including extensions
#include <GL/glut.h>
#include "scene.h"

// -----------------------------------
// Const variables
extern const int shadowMapSize;
extern const float white[3];
extern const float black[3];
// Variables
extern GLuint g_shadowMapTexture;
extern int g_windowWidth, g_windowHeight;
extern float g_lightProjectionMatrix[16], g_lightViewMatrix[16];
extern float g_cameraProjectionMatrix[16], g_cameraViewMatrix[16];

// TODO REMOVE THIS !
//Camera & light positions
extern float cameraPosition[4];
extern float lightPosition[4];


// -----------------------------------
// Fonctions
int initShadow();
