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

#include "shadow.h"

//Size of shadow map
const int shadowMapSize = 512;
const float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

//Textures
GLuint g_shadowMapTexture;

//window size
int g_windowWidth, g_windowHeight;

//Matrices
float g_lightProjectionMatrix[16], g_lightViewMatrix[16];
float g_cameraProjectionMatrix[16], g_cameraViewMatrix[16];

// TODO REMOVE THIS !
//Camera & light positions
float lightPosition[3]  = {2.0f, 3.0f, 2.0f};


//Called for initiation
int initShadow() {
    //Check for necessary extensions
    if(!GLEE_ARB_depth_texture || !GLEE_ARB_shadow) {
        printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
        return -1;
    }

    //Load identity modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Shading states
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //Depth states
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    //We use glScale when drawing the scene
    glEnable(GL_NORMALIZE);

    //Create the shadow map texture
    glGenTextures(1, &g_shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, g_shadowMapTexture);
    glTexImage2D(   GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
                    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Use the color as the ambient and diffuse material
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL); // no need

    //White specular material color, shininess 16
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

    //Calculate & save matrices
    glPushMatrix();

    glLoadIdentity();
    gluPerspective(45.0f, (float)g_windowWidth/g_windowHeight, 1.0f, 100.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_cameraProjectionMatrix);

    recomputeLightMatrices();

    return 0;
}

void saveCameraMatrix() {
    glGetFloatv(GL_MODELVIEW_MATRIX, g_cameraViewMatrix);

    glPopMatrix();
}

void recomputeLightMatrices() {
    glPushMatrix();

    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_lightProjectionMatrix);

    glLoadIdentity();
    gluLookAt( lightPosition[0], lightPosition[1], lightPosition[2],
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_lightViewMatrix);

    glPopMatrix();
}
