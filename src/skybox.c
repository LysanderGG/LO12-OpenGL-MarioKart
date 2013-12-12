#include <stdlib.h>
#include <stdio.h>

#include "scene.h"
#include <string.h>
#include "pngloader.h"

#if defined(__APPLE__) && defined(__MACH__)
        #include <GLUT/glut.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
#else
        #include <GL/glut.h>
        #include <GL/gl.h>
        #include <GL/glu.h>
#endif

GLuint *textureBox;
GLdouble cx, cy, cz; //position camera
    

//******************************************************************************************
void calcul_pos_cam() {
    GLdouble  model[16], proj[16];
    GLint viewport[4],res;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    res=gluUnProject( (viewport[2]-viewport[0])/2 , (viewport[3]-viewport[1])/2, 0.0, 
                   model, proj, viewport,  
                   &cx,&cy,&cz);
}

//*****************************************************************************************************************************
int charger_skybox(void) {
    int i, j;
    MTEX *texture = (MTEX *)malloc(6*sizeof(MTEX));    
    textureBox = (GLuint *)malloc(6*sizeof(GLuint));
    glGenTextures(6, textureBox);

    texture[0].filename="img/XN.png";
    texture[1].filename="img/XP.png";
    texture[2].filename="img/YN.png";
    texture[3].filename="img/YP.png";
    texture[4].filename="img/ZN.png";
    texture[5].filename="img/ZP.png";

    for(i=0;i<6;i++) 
    {        
        if(ReadPNGFromFile (&texture[i])) {
                printf("Je n'arrive pas à lire %s\n",texture->filename);
                return -1;
        }
        /* à compléter */
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );                    
    }
    free(texture);    
    return 0;
}

//*****************************************************************************************************************************
void dessine_box() {
    float t = 1.0f;
    // Configuration des états OpenGL
    glDisable(GL_LIGHTING);
    
    // Désactivation de l'ecriture dans le z-buffer
    glDisable(GL_DEPTH_TEST);
    

    // Réglage de la position de la box
    /* à compléter */
    //glPushMatrix();
    //glLoadIdentity();
    
    

    // Rendu de la geometrie
    glBindTexture(GL_TEXTURE_2D, textureBox[0]);
    glBegin(GL_QUADS);            // X Negatif        
        glTexCoord2f(0.0, 0.0); glVertex3f(-t, -t, -t);
        glTexCoord2f(1.0, 0.0); glVertex3f(-t, t, -t);
        glTexCoord2f(1.0, 1.0); glVertex3f(-t, t, t);
        glTexCoord2f(0.0, 1.0); glVertex3f(-t, -t, t);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureBox[1]);
    glBegin(GL_QUADS);            // X Positif
        glTexCoord2f(0.0, 0.0); glVertex3f(t, t, -t);
        glTexCoord2f(1.0, 0.0); glVertex3f(t, -t, -t);
        glTexCoord2f(1.0, 1.0); glVertex3f(t, -t, t);
        glTexCoord2f(0.0, 1.0); glVertex3f(t, t, t);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureBox[2]);
    glBegin(GL_QUADS);            // Y Negatif
        glTexCoord2f(0.0, 0.0); glVertex3f(-t, -t, -t);
        glTexCoord2f(1.0, 0.0); glVertex3f(-t, -t, t);
        glTexCoord2f(1.0, 1.0); glVertex3f(t, -t, -t);
        glTexCoord2f(0.0, 1.0); glVertex3f(t, -t, t);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureBox[3]);
    glBegin(GL_QUADS);            // Y Positif
        glTexCoord2f(0.0, 0.0); glVertex3f(-t, t, -t);     
        glTexCoord2f(1.0, 0.0); glVertex3f(t, t, -t);
        glTexCoord2f(1.0, 1.0); glVertex3f(-t, t, t);
        glTexCoord2f(0.0, 1.0); glVertex3f(t, t, t);     
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureBox[4]);
    glBegin(GL_QUADS);            // Z Negatif
        glTexCoord2f(0.0, 0.0); glVertex3f(-t, -t, -t);     
        glTexCoord2f(1.0, 0.0); glVertex3f(t, -t, -t);
        glTexCoord2f(1.0, 1.0); glVertex3f(-t, t, -t);
        glTexCoord2f(0.0, 1.0); glVertex3f(t, t, -t);     
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textureBox[5]);
    glBegin(GL_QUADS);            // Z Positif
        glTexCoord2f(0.0, 0.0); glVertex3f(-t, -t, t);     
        glTexCoord2f(1.0, 0.0); glVertex3f(-t, t, t);
        glTexCoord2f(1.0, 1.0); glVertex3f(t, -t, t);
        glTexCoord2f(0.0, 1.0); glVertex3f(t, t, t);     
    glEnd();
    
    // Réactivation de l'écriture dans le z-buffer
    glEnable(GL_DEPTH_TEST);
    
    // Réinitialisation des états OpenGL    
    glEnable(GL_LIGHTING);
}

