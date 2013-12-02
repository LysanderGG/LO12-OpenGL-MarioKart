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
void calcul_pos_cam()
{
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
int charger_skybox(void)
{
    int i, j;
    MTEX *texture=(MTEX *)malloc(6*sizeof(MTEX));	
	textureBox=(GLuint *)malloc(6*sizeof(GLuint));
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
                printf("Je n'arrive pas � lire %s\n",texture->filename);
                return -1;
        }
        /* � compl�ter */
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );        			
    }
	free(texture);	
    return 0;
}
//*****************************************************************************************************************************
void dessine_box()
{
	float t=1.0;
	// Configuration des �tats OpenGL
	
	/* � compl�ter */ // D�sactivation de l'ecriture dans le z-buffer
	/* � compl�ter */
	

	// R�glage de la position de la box
	/* � compl�ter */
	

	// Rendu de la g�om�trie
	glBindTexture(GL_TEXTURE_2D, textureBox[0]);
	glBegin(GL_QUADS);			// X N�gatif		
		glTexCoord2f(0.0, 0.0); glVertex3f(-t,-t,-t); 	
		glTexCoord2f(1.0, 0.0); glVertex3f(-t,t,-t);
		glTexCoord2f(1.0, 1.0); glVertex3f(-t,t,t);
		glTexCoord2f(0.0, 1.0); glVertex3f(-t,-t,t); 
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureBox[1]);
	glBegin(GL_QUADS);			// X Positif
		glTexCoord2f(0.0, 0.0); glVertex3f(t,t,-t); 	
		glTexCoord2f(1.0, 0.0); glVertex3f(t,-t,-t);
		glTexCoord2f(1.0, 1.0); glVertex3f(t,-t,t);
		glTexCoord2f(0.0, 1.0); glVertex3f(t,t,t); 	
	glEnd();

	/* � compl�ter */

	
	
	// R�activation de l'�criture dans le z-buffer
	/* � compl�ter */
	
	// R�initialisation des �tats OpenGL	
	/* � compl�ter */
}

