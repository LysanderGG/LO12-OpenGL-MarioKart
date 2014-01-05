
#ifdef _MSC_VER
//#define TEMPO_WIN32
#else
#define TEMPO_UNIX
#endif

#ifdef TEMPO_UNIX
#include <unistd.h> /*Temporisation pour unix*/
#endif

#include <windows.h> /*Temporisation pour windows*/
#include <stdio.h>

#include "GLee/GLee.h" //GL header file, including extensions
#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "affiche.h"
#include "animations.h"
#include "eclairage.h"
#include "interactions.h"
#include "scene3ds.h"
#include "shadow.h"
#include "skybox.h"
#include "Utils.h"

extern SCENE* scene;

void DrawScene_sample(float angle);

/*******************************************************************************/
/*            Fonctions pour afficher le contenu de la structure scene            */
/*******************************************************************************/

void dessine_face(INDICE_FACE iface, int _differentColorForEachVertex)
{
    int i;          // compteur de point
    int j;          // indice de point
    int idxTexture; // indice de la texture
    MCOORD c, d;
    

    // TODO : faire une fonction
    // calcul et dessin de la normale
    c.x = c.y = c.z = 0;
    for(i = 0; i < scene->tabface[iface].nbpt ; ++i) {
        j = scene->tabface[iface].tabpt[i];
        c.x += scene->tabpt[j].x;
        c.y += scene->tabpt[j].y;
        c.z += scene->tabpt[j].z;
    }
    c.x /= 4.0;
    c.y /= 4.0;
    c.z /= 4.0;

    d.x = c.x + scene->tabface[iface].n.x;
    d.y = c.y + scene->tabface[iface].n.y;
    d.z = c.z + scene->tabface[iface].n.z;

    glDisable(GL_LIGHTING);
    glColor3f(.0f, .0f, .0f);
    glBegin(GL_LINES);
    glVertex3dv(c.vect);
    glVertex3dv(d.vect);
    glEnd();
    glEnable(GL_LIGHTING);

    // Fin dessin de la normale

    if(g_switchLight) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    if(g_isCurrentObject) {
        def_selectedMatiere(scene, scene->tabface[iface].imat);
    } else {
        def_matiere(scene, scene->tabface[iface].imat);
    }

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glNormal3dv(scene->tabface[iface].n.vect);

    for(i = 0 ; i < scene->tabface[iface].nbpt ; ++i) {
        j = scene->tabface[iface].tabpt[i];
        idxTexture = scene->tabface[iface].tabtc[i];

        glTexCoord2d(scene->tabtc[idxTexture].x, scene->tabtc[idxTexture].y);
        glVertex3d(scene->tabpt[j].x, scene->tabpt[j].y, scene->tabpt[j].z);
    }

    glEnd();
    glDisable( GL_TEXTURE_2D);
}

/*****************************************************************************/
void dessine_objet(OBJET objet)
{
    int  i;      /* compteur de faces */

    for (i = 0 ; i < objet.nbface ; ++i) {
        glPushMatrix();
        glMultMatrixd(objet.transfo);
        dessine_face(objet.tabface[i], 0);
#ifdef TEMPO_UNIX
            sleep(1);
            glutSwapBuffers();
#endif
#ifdef TEMPO_WIN32
            Sleep(1000);
            glutSwapBuffers();
#endif
        glPopMatrix();
    }

}


/****************************************************************************/
void dessine_scene() {
    int i;  /* indice d'objet */
    int currentSceneObject = 0;

    //printf("==============================\n");
    //printf("========= dessine_scene ======\n");
    //printf("==============================\n");

    // Reinitialisation des variables globales utilisee dans dessine_scene
    g_current3DSScene = 0;

    calcule_normales(scene); // A optimiser peut etre
    
    i = 0;
    while(i < getTotalNbObjects()) {
        //printf("-------------------\n");
        //printf("Iteration i = %d\n", i);
        //printf("g_current3DSScene = %d\n", g_current3DSScene);
        //printf("currentSceneObject = %d\n", currentSceneObject);

        if(i == g_currentObj) {
            g_isCurrentObject = 1;
        } else {
            g_isCurrentObject = 0;
        }

        if(i < scene->nbobj) {
            // Objets definis dans le .dat
            dessine_objet(scene->tabobj[i]);
        } else {
            // Objets definis dans un fichier 3DS

            // S'il n'y a plus rien a dessiner on arrete
            if(g_current3DSScene >= NB_MAX_3DS_SCENES) {
                break;
            }
                
            // Si la scene g_current3DSScene n'est pas/plus definie, on passe a la suivante
            if(g_scenes3DS[g_current3DSScene].lib3dsfile == NULL) {
                ++g_current3DSScene;
                continue;
            }

            // Si on a dessine tous les objets de la scene 3DS, on passe a la prochaine scene
            if(currentSceneObject == g_scenes3DS[g_current3DSScene].lib3dsfile->nmeshes) {
                // Passe a la scene suivante
                currentSceneObject = 0;
                ++g_current3DSScene;
                continue;
            }

            // Scene 3ds
            // On dessine le currentSceneDrawnObjects eme objet de la scene 3DS
            dessine_3dsobj(&g_scenes3DS[g_current3DSScene], g_scenes3DS[g_current3DSScene].lib3dsfile->meshes[currentSceneObject++]);
        }
        ++i;
    }
}

/****************************************************************************/

/****************************************************************************/
void dessine_repere() {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(1.0f, .0f, .0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    glColor3f(0.5f, .0f, .0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-10.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, .0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    glColor3f(.0f, .0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}
/****************************************************************************/

/****************************************************************************/
void reshape(int _w, int _h) {
    if(_h < 1) { _h = 1; }

    //Save new window size
    g_windowWidth = _w, g_windowHeight = _h;

    glPushMatrix();
    glLoadIdentity();
    gluPerspective(DEFAULT_PERSPECTIVE_FOVY, (GLdouble)_w / _h, DEFAULT_PERSPECTIVE_NEAR, DEFAULT_PERSPECTIVE_FAR);
    glGetFloatv(GL_MODELVIEW_MATRIX, g_cameraProjectionMatrix);
    glPopMatrix();
}
/****************************************************************************/

void idleFunc() {
    Sleep(10);
    glutPostRedisplay();
}

void redefineLights() {
    g_nbLights  = 0;
    g_nextLight = GL_LIGHT0;
    def_sources(scene);
    def3DSSources(g_scenes3DS);
    
}

void draw2DText(char* str) {
    unsigned int i;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(-1.0f, 0.9f);
    for(i = 0; i < strlen(str); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void draw() {
    // First handles user inputs
    handleKeyboardEvents();

    // Clean screan
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    dessine_box();

    drawSceneWithShadow();

    if(g_debugRepere) {
        dessine_repere();
    }

    // Dessine les sources lumineuses (vecteurs dans la direction de la source).
    if(g_debugLights) {
        draw3DSLights(g_scenes3DS);
    }

    glFinish();
    glutSwapBuffers();
    glutPostRedisplay();
}

// Draw with Shadows
void drawSceneWithShadow(void) {
    float textureMatrix[16];
    float row[4];
    float white2[3];
    float lightPosition4D[4];
    int b_dessine_scene  = 1;
    int b_dessine_sample = 0;

    static float biasMatrix[16] = { 0.5f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.5f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.5f, 0.0f,
                                    0.5f, 0.5f, 0.5f, 1.0f }; //bias from [-1, 1] to [0, 1]

    glEnable(GL_TEXTURE_2D);

    // -------------------------------------------------
    // First pass - from light's point of view
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(g_lightProjectionMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(g_lightViewMatrix);
    
    //Use viewport the same size as the shadow map
    glViewport(0, 0, shadowMapSize, shadowMapSize);

    //Draw back faces into the shadow map
    glCullFace(GL_FRONT);

    //Disable color writes, and use flat shading for speed
    glShadeModel(GL_FLAT);
    glColorMask(0, 0, 0, 0);

    //Draw the scene
    if(b_dessine_sample) {
        DrawScene_sample(2.5);
    }
    if(b_dessine_scene) {
        dessine_scene();
    }

    //Read the depth buffer into the shadow map texture
    glBindTexture(GL_TEXTURE_2D, g_shadowMapTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

    //restore states
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glColorMask(1, 1, 1, 1);
    


    // -------------------------------------------------
    //2nd pass - Draw from camera's point of view

    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(g_cameraProjectionMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(g_cameraViewMatrix);

    glViewport(0, 0, g_windowWidth, g_windowHeight);
    
    //Use dim light to represent shadowed areas
    white2[0] = white[0] * 0.2f;
    white2[1] = white[1] * 0.2f;
    white2[2] = white[2] * 0.2f;
    lightPosition4D[0] = lightPosition[0];
    lightPosition4D[1] = lightPosition[1];
    lightPosition4D[2] = lightPosition[2];
    lightPosition4D[3] = 0;
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition4D);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  white2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  white2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, black);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    if(b_dessine_sample) {
        DrawScene_sample(2.5);
    }
    if(b_dessine_scene) {
        dessine_scene();
    }
    
    // -------------------------------------------------
    //3rd pass
    //Draw with bright light
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white2);

    //Calculate texture matrix for projection
    //This matrix takes us from eye space to the light's clip space
    //It is postmultiplied by the inverse of the current view matrix when specifying texgen
    
    //textureMatrix = biasMatrix * g_lightProjectionMatrix * g_lightViewMatrix;
    matrix4x4Product(g_lightProjectionMatrix, g_lightViewMatrix, textureMatrix);
    matrix4x4Product(biasMatrix, textureMatrix, textureMatrix);

    //Set up texture coordinate generation.

    // row 0
    getMatrix4x4Row(textureMatrix, row, 0);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, row);
    glEnable(GL_TEXTURE_GEN_S);

    // row 1
    getMatrix4x4Row(textureMatrix, row, 1);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE, row);
    glEnable(GL_TEXTURE_GEN_T);

    // row 2
    getMatrix4x4Row(textureMatrix, row, 2);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, row);
    glEnable(GL_TEXTURE_GEN_R);

    // row 3
    getMatrix4x4Row(textureMatrix, row, 3);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, row);
    glEnable(GL_TEXTURE_GEN_Q);

    //Bind & enable shadow map texture
    glBindTexture(GL_TEXTURE_2D, g_shadowMapTexture);
    
    //Enable shadow comparison
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
    
    //Shadow comparison should be true (ie not in shadow) if r<=texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    
    //Shadow comparison should generate an INTENSITY result
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

    //Set alpha test to discard false comparisons
    glAlphaFunc(GL_GEQUAL, 0.99f);
    glEnable(GL_ALPHA_TEST);
    
    if(b_dessine_sample) {
        DrawScene_sample(2.5);
    }
    if(b_dessine_scene) {
        dessine_scene();
    }
    
    //Disable textures and texgen
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    
    //Restore other states
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
}






void DrawScene_sample(float angle) {
    //Display lists for objects
    static GLuint spheresList=0, torusList=0, baseList=0;

    //Create spheres list if necessary
    if(!spheresList) {
        spheresList=glGenLists(1);
        glNewList(spheresList, GL_COMPILE);
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            glPushMatrix();

            glTranslatef(0.45f, 1.0f, 0.45f);
            glutSolidSphere(0.2, 24, 24);

            glTranslatef(-0.9f, 0.0f, 0.0f);
            glutSolidSphere(0.2, 24, 24);

            glTranslatef(0.0f, 0.0f,-0.9f);
            glutSolidSphere(0.2, 24, 24);

            glTranslatef(0.9f, 0.0f, 0.0f);
            glutSolidSphere(0.2, 24, 24);

            glPopMatrix();
        }
        glEndList();
    }

    //Create torus if necessary
    if(!torusList) {
        torusList=glGenLists(1);
        glNewList(torusList, GL_COMPILE);
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glPushMatrix();

            glTranslatef(0.0f, 0.5f, 0.0f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glutSolidTorus(0.2, 0.5, 24, 48);

            glPopMatrix();
        }
        glEndList();
    }

    //Create base if necessary
    if(!baseList) {
        baseList=glGenLists(1);
        glNewList(baseList, GL_COMPILE);
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glPushMatrix();

            glScalef(1.0f, 0.05f, 1.0f);
            glutSolidCube(3.0f);

            glPopMatrix();
        }
        glEndList();
    }

    //Draw objects
    glCallList(baseList);
    glCallList(torusList);
    
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glCallList(spheresList);
    glPopMatrix();
}
