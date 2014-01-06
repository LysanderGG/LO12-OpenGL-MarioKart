
#ifndef SCENE_H
#define SCENE_H

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


typedef GLint INDICE_PT;       /* indice de poGLint */
typedef GLint INDICE_MAT;      /* indice de matiere */
typedef GLint INDICE_FACE;     /* indice de face */
typedef GLint INDICE_TEX;
typedef GLint INDICE_TEXCOORD; /* indice de coordonées de texture*/

/*---------------------------------------------------------------------*/


typedef union {
    struct {
        GLdouble x;
        GLdouble y;
    };
    GLdouble vect[2];
} MTEXCOORD;

typedef struct {
    char *filename;
    GLint width;
    GLint height;
    GLenum format;
    GLint internalFormat;
    GLubyte *texels;
    GLuint glnum;
} MTEX;

typedef union {
    struct {
        GLdouble x;
        GLdouble y;
        GLdouble z;
        GLdouble w;
    };
    GLdouble vect[4];
} MCOORD;

typedef union {
    struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;
    };
    GLfloat vect[4];
} MCOORDF;

/*---------------------------------------------------------------------*/

typedef GLdouble MTRANSFO[16];   /* matrice de transformation homogene 4x4 */
/* compatibilit�OpenGL : vecteur 16    */

/*---------------------------------------------------------------------*/

typedef union {
    struct {
        GLdouble r;     /* rouge */
        GLdouble g;     /* vert */
        GLdouble b;     /* bleu */
    };
    GLdouble rgb[3];
} RGB;

typedef union {
    struct {
        GLdouble r;     /* rouge */
        GLdouble g;     /* vert */
        GLdouble b;     /* bleu */
        GLdouble a;     /* alpha*/
    };
    GLdouble rgba[4];
} RGBA;


typedef union {
    struct {
        GLfloat r;     /* rouge */
        GLfloat g;     /* vert */
        GLfloat b;     /* bleu */
    };
    GLfloat rgb[3];
} RGBF;

typedef union {
    struct {
        GLfloat r;     /* rouge */
        GLfloat g;     /* vert */
        GLfloat b;     /* bleu */
        GLfloat a;     /* alpha*/
    };
    GLfloat rgba[4];
} RGBAF;

/*---------------------------------------------------------------------*/

typedef struct {
    GLint       nbpt;       /* nombre de points constituant la face */
    INDICE_PT   *tabpt;     /* tableau des indice de poGLints constituant la face */
    INDICE_MAT  imat;       /* indice de matiere */
    MCOORD      n;          /* normale a la face */
    GLint       nbtc;
    INDICE_TEXCOORD *tabtc;
} FACE;

/*---------------------------------------------------------------------*/

typedef struct {
    GLdouble    alpha;          /* coeff. de transparence */
    RGB         ambiante;       /* couleur de la reflectance ambiante */
    RGB         diffuse;        /* couleur de la reflectance diffuse */
    RGB         emission;       /* couleur de la reflectance emise */
    RGB         speculaire;     /* couleur de la reflectance speculaire */
    GLdouble    brillance;      /* coeff. de brillance */
    INDICE_TEX  tex;
} MATIERE;

/*-----------------------------------------------------------------------*/

typedef struct {
    GLint       nbface;     /* nombre de faces constituant l'objet */
    INDICE_FACE *tabface;   /* tableau des faces constituant l'objet */
    MTRANSFO    transfo;    /* transfo du repere objet/repere scene */
} OBJET;

/*------------------------------------------------------------------------*/

typedef struct {
    GLdouble k;
    GLdouble theta;
} ALLURE_FAISCEAU;

/*-----------------------------------------------------------------------*/

typedef struct {
    RGB             ambiante;           /* couleur "ambiente" */
    RGB             couleur;            /* couleur de la source */
    MCOORD          position;           /* position de la source */
    ALLURE_FAISCEAU allure_faisceau;    /* allure du faisceau lumineux */
    MCOORD          direction;          /* direction du faisceau lumineux */
    GLint           allume;
} SOURCE;

/*-------------------------------------------------------------------------*/

typedef struct {
    GLint       nbpt;        /* nombre de points                */
    MCOORD*     tabpt;       /* tableau des points              */
    GLint       nbface;      /* nombre de faces                 */
    FACE*       tabface;     /* tableau des faces               */
    GLint       nbmat;       /* nombre de matieres              */
    MATIERE*    tabmat;      /* tableau des matieres            */
    GLint       nbobj;       /* nombre d'objets                 */
    OBJET*      tabobj;      /* tableau des objets              */
    GLint       nbsource;    /* nombre de sources de lumiere    */
    SOURCE*     tabsource;   /* tableau des sources de lumiere  */
    MTRANSFO    transfo;
    GLint       nbtex;
    MTEX*       tabtex;
    GLint       nbtc;
    MTEXCOORD*  tabtc;
    GLuint*     gltextab;
} SCENE;

/*---------------------------------------------------------------------------*/

//*********************//
//Defines :            //
//*********************//

#define KART_ID                 0
#define ANIMATED_KART_ID        1
#define NB_MAX_3DS_SCENES       100
#define NB_MAX_3DS_MODELS       10
#define NB_MIN_RANDOM_OBJECTS   5
#define NB_MAX_RANDOM_OBJECTS   20
#define SCENE_X_MIN             -100
#define SCENE_X_MAX             +100
#define SCENE_Y_MIN             -100
#define SCENE_Y_MAX             +100

#define DEFAULT_PERSPECTIVE_FOVY    45.0
#define DEFAULT_PERSPECTIVE_NEAR    1.0
#define DEFAULT_PERSPECTIVE_FAR     1000.0

#define USE_SHADOW_MODEL        0

//*********************//
//Variables Globales : //
//*********************//

extern SCENE*       scene;          /* description de la scene */

//*********************//
//Fonctions :          //
//*********************//
int getTotalNbObjects();


#endif
