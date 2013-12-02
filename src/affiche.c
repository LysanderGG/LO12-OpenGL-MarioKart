/*
 
        Universite Technologique de Compiegne
        
        UV: LO12
        
        FICHIER: affiche.c
 
        COMMENTAIRE:
                Routines d'affichage
 
        AUTEURS:
                Veronique BERGE-CHERFAOUI
                DG
                Olivier BEZET  A2002-A2005
                Romain HERAULT A2005-A2006
*/

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
#include "eclairage.h"
#include "interactions.h"

extern SCENE* scene;

static float g_rotateAngle = 0.0f;

/*******************************************************************************/
/*            Fonctions pour afficher le contenu de la structure scene			*/
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
    
    /*if(g_isCurrentObject) {
        // TODO : donner une matiere différente trop stylee
        glDisable(GL_LIGHTING);
		glColor3f(1.0f, .0f, .0f);
    } else {*/
        def_matiere(scene, scene->tabface[iface].imat);
    //}

        glEnable( GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glNormal3dv(scene->tabface[iface].n.vect);

    for(i = 0 ; i < scene->tabface[iface].nbpt ; ++i) {
        j = scene->tabface[iface].tabpt[i];
        idxTexture = scene->tabface[iface].tabtc[i];

        glTexCoord2d(scene->tabtc[idxTexture].x, scene->tabtc[idxTexture].y);
        glVertex3d(scene->tabpt[j].x, scene->tabpt[j].y, scene->tabpt[j].z);
    }

    /*if(g_isCurrentObject) {
        glEnable(GL_LIGHTING);
    }*/
    

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
void dessine_scene()
{
    int i;  /* indice d'objet */

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // efface l'ecran

    dessine_repere();

    calcule_normales(scene); // A optimiser peut etre
    def_sources(scene);
		
    //glIndexi(2);		 /* couleur de la scene en mode index */
    for(i = 0 ; i < scene->nbobj; ++i) {
		if(i == g_currentObj) {
            g_isCurrentObject = 1;
        } else {
            g_isCurrentObject = 0;
        }

		dessine_objet(scene->tabobj[i]);
    }

    glutSwapBuffers();
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
	glViewport(0, 0, _w, _h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0 , (GLdouble)_w / _h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
/****************************************************************************/

void idleFunc() {
	Sleep(10);
	g_rotateAngle += 0.5f;
	glutPostRedisplay();
}