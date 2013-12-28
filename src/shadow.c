#include "shadow.h"

//Size of shadow map
const int shadowMapSize=512;
const float white[3] = { 1.0f, 1.0f, 1.0f };
const float black[3] = { 0.0f, 0.0f, 0.0f };

//Textures
GLuint g_shadowMapTexture;

//window size
int g_windowWidth, g_windowHeight;

//Matrices
float g_lightProjectionMatrix[16], g_lightViewMatrix[16];
float g_cameraProjectionMatrix[16], g_cameraViewMatrix[16];

// TODO REMOVE THIS !
//Camera & light positions
float cameraPosition[4] = {-2.5f, 3.5f,-2.5f, 1.0f};
float lightPosition[4]  = {2.0f, 3.0f,-2.0f, 1.0f};


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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Enable shadow comparison
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    //Shadow comparison should be true (ie not in shadow) if r<=texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    //Shadow comparison should generate an INTENSITY result
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    //Use the color as the ambient and diffuse material
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    //White specular material color, shininess 16
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

     //Calculate & save matrices
    glPushMatrix();

    glLoadIdentity();
    gluPerspective(45.0f, (float)g_windowWidth/g_windowHeight, 1.0f, 100.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_cameraProjectionMatrix);

    glLoadIdentity();
    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_cameraViewMatrix);

    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_lightProjectionMatrix);

    glLoadIdentity();
    gluLookAt( lightPosition[0], lightPosition[1], lightPosition[2],
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_lightViewMatrix);

    glPopMatrix();

    return 0;
}
