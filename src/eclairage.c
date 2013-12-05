/*
 
        Universite Technologique de Compiegne
        
        UV: LO12
        
        FICHIER: eclairage.c
 
        COMMENTAIRE:
                Routines d'eclairage
 
        AUTEURS:
                Veronique BERGE-CHERFAOUI
                DG
                Olivier BEZET  A2002-A2005
                Romain HERAULT A2005-A2006
*/

#include <math.h>
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

#include "eclairage.h"
#include "scene.h"

extern SCENE* scene;

// Prototypes
void calcule_normale(SCENE *scene, int i);


/*******************************************************************************/
/*                         Mise en place de l'eclairage                        */
/*******************************************************************************/

/* -----------------------------------------------
Cette fonction realise la norme euclidienne d'un
vecteur v.
On ne prend pas en compte la quatrieme composante.
----------------------------------------------- */

GLdouble norme(MCOORD v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/* -----------------------------------------------
Cette fonction realise le produit vectoriel de deux
vecteurs
  w = u ^ v
----------------------------------------------- */

void prod_vectoriel(MCOORD u, MCOORD v, MCOORD *w) {
    w->x = u.y * v.z - u.z * v.y;
    w->y = u.x * v.z - u.z * v.x;
    w->z = u.x * v.y - u.y * v.x;
}

void calcule_vecteur(MCOORD p1, MCOORD p2, MCOORD *v) {
    v->x = p2.x - p1.x;
    v->y = p2.y - p1.y;
    v->z = p2.z - p1.z;
}

void normalise(MCOORD *v) {
    GLdouble norm = norme(*v);
    if(norm > 0.0000001) {
        v->x /= norm;
        v->y /= norm;
        v->z /= norm;
    } else {
        v->x = 0.0;
        v->y = 0.0;
        v->z = 0.0;
    }
}


/*****************************************************************************/
void calcule_normales(SCENE *scene) {
    int i = 0;
    for(i = 0; i < scene->nbface; ++i) {
        calcule_normale(scene, i);
    }
}

void calcule_normale(SCENE *scene, int i) {
    MCOORD v1, v2;
    
    calcule_vecteur( scene->tabpt[scene->tabface[i].tabpt[0]]
                   , scene->tabpt[scene->tabface[i].tabpt[1]]
                   , &v1);
    calcule_vecteur( scene->tabpt[scene->tabface[i].tabpt[0]]
                   , scene->tabpt[scene->tabface[i].tabpt[scene->tabface[i].nbpt-1]]
                   , &v2);
    prod_vectoriel(v1, v2, &scene->tabface[i].n);

    normalise(&scene->tabface[i].n);
}


/**********************************************************************/
GLenum lumiere_i(int i) {
    GLenum source;
    switch (i) {
    case 0:
        source = GL_LIGHT0;
        break;
    case 1:
        source = GL_LIGHT1;
        break;
    case 2:
        source = GL_LIGHT2;
        break;
    case 3:
        source = GL_LIGHT3;
        break;
    case 4:
        source = GL_LIGHT4;
        break;
    case 5:
        source = GL_LIGHT5;
        break;
    case 6:
        source = GL_LIGHT6;
        break;
    case 7:
        source = GL_LIGHT7;
        break;
    }

    if(i <= 7)
        return(source);
    else {
        printf("Attention: nombre de sources lumineuses depasse\n");
        return ((GLenum)GL_LIGHT7);
    }
}


void rgb2rgbaf(RGB *rgb, GLfloat alpha, RGBAF *rgbaf) {
    rgbaf->r = rgb->r;
    rgbaf->g = rgb->g;
    rgbaf->b = rgb->b;
    rgbaf->a = alpha;
}

void mcoord2mcoordf(MCOORD *mcoord,MCOORDF *mcoordf) {
    mcoordf->x = mcoord->x;
    mcoordf->y = mcoord->y;
    mcoordf->z = mcoord->z;
    mcoordf->w = mcoord->w;
}

/*****************************************************************************/
void def_sources(SCENE *scene) {
    RGBAF propc;    /* tableau des proprietes de la source */
    MCOORDF propp;
    GLenum source;
    int i = 0;

    for(i = 0; i < scene->nbsource; ++i) {
        source = lumiere_i(i);

        if(scene->tabsource[i].allume) {
            rgb2rgbaf(&scene->tabsource[i].ambiante, 1, &propc);
            glLightfv(source,GL_AMBIENT,propc.rgba);

            rgb2rgbaf(&scene->tabsource[i].couleur, 1, &propc);
            glLightfv(source,GL_DIFFUSE,propc.rgba);
            glLightfv(source,GL_SPECULAR,propc.rgba);

            mcoord2mcoordf(&scene->tabsource[i].position, &propp);
            glLightfv(source,GL_POSITION,propp.vect);

            glLightf(source,GL_SPOT_CUTOFF, scene->tabsource[i].allure_faisceau.theta);
            glLightf(source,GL_SPOT_EXPONENT, scene->tabsource[i].allure_faisceau.k);

            mcoord2mcoordf(&scene->tabsource[i].direction, &propp);
            glLightfv(source,GL_SPOT_DIRECTION,propp.vect);

            glEnable(source);
        } else {
            glDisable(source);
        }
    }
}

/*****************************************************************************/
void def_modele(SCENE *scene) {
        RGBAF propc;

        propc.r = 0.2;
        propc.g = 0.2;
        propc.b = 0.2;
        propc.a = 1.0;
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,propc.rgba);

        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}



/*****************************************************************************/
void def_matiere(SCENE *scene, int i) {
    RGBAF propc;
    INDICE_TEX texIndex;

    // Couleur
    rgb2rgbaf(&scene->tabmat[i].ambiante, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, propc.rgba);

    rgb2rgbaf(&scene->tabmat[i].diffuse, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, propc.rgba);

    rgb2rgbaf(&scene->tabmat[i].speculaire, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, propc.rgba);

    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, scene->tabmat[i].brillance);

    rgb2rgbaf(&scene->tabmat[i].emission, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, propc.rgba);

    // Texture
    texIndex = scene->tabmat[i].tex;
    glBindTexture(GL_TEXTURE_2D, scene->tabtex[texIndex].glnum);
}

void def_selectedMatiere(SCENE *scene, int i) {
    INDICE_TEX texIndex;
    RGBAF propc;

    // Couleur
    propc.r = 0.4f;
    propc.g = 0.0f;
    propc.b = 0.0f;
    propc.a = 0.4f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , propc.rgba);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, scene->tabmat[i].brillance);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION , propc.rgba);

    // Texture
    texIndex = scene->tabmat[i].tex;
    glBindTexture(GL_TEXTURE_2D, scene->tabtex[texIndex].glnum);
}
