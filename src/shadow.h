// Original code from :
//////////////////////////////////////////////////////////////////////////////////////////
//  Shadow Mapping Tutorial
//  Accompanies a tutorial found on my site
//  Downloaded from: www.paulsprojects.net
//  Created: 16th September 2003
//
//  Copyright (c) 2006, Paul Baker
//  Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//  http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////    
//
// Adapted by Guillaume George
// 29th December 2013
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "GLee/GLee.h" //GL header file, including extensions
#include <GL/glut.h>
#include "scene.h"

// -----------------------------------
// Const variables
extern const int shadowMapSize;
extern const float white[4];
extern const float black[4];
// Variables
extern GLuint g_shadowMapTexture;
extern int g_windowWidth, g_windowHeight;
extern float g_lightProjectionMatrix[16], g_lightViewMatrix[16];
extern float g_cameraProjectionMatrix[16], g_cameraViewMatrix[16];

// TODO REMOVE THIS !
//Camera & light positions
extern float lightPosition[3];


// -----------------------------------
// Fonctions
int initShadow();
void saveCameraMatrix();
void recomputeLightMatrices();