/*
 
    Universite Technologique de Compiegne
        
    UV: LO12
        
    FICHIER: tp1.c
 
    COMMENTAIRE:
            Fichier principal
            Interface OpenGL (GLUT, GLU, GL)
 
    AUTEURS:
            Veronique BERGE-CHERFAOUI
            Olivier BEZET  A2002-A2005
            Romain HERAULT A2005-A2006
*/

//******************//
//Fichiers inclus : //
//******************//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "SceneUtils.h"
#include "affiche.h"
#include "observateur.h"
#include "animations.h"
#include "interactions.h"
#include "texture.h"
#include "scene3ds.h"
#include "skybox.h"

//**************//
//DEFINITIONS : //
//**************//

#define PROJECT_NAME "scene"
#define EXT_3DS "_3ds"
#define FILE_EXT ".dat"

#define SCENE_FILE_3DS "img/3d/kartPhare.3ds"


/*******************************************************************************/
/*                      Initialisations pour GL et Glut                        */
/*******************************************************************************/
void init()
{
    glEnable(GL_DEPTH_TEST | GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,1.0,0.1,100) ;

    initView();

    /* initialise la table des couleurs : mode index
        glutSetColor(1,0.0, 0.0, 0.0);
        glutSetColor(2,1.0, 0.3, 0.3);
        choix de la couleur de fond 
        glClearIndex(1.0);*/

    /* mode RGB : choix de la couleur de fond */
    glClearColor(0,0,0,1);

    /* applique la couleur de fond */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    def_modele(scene);
    charger_skybox();
    loadTextures();
}

/* ========================================================================= */

int main(int argc, char**argv) {
    char  default_filename[80] = PROJECT_NAME FILE_EXT;
    char  default_filename_3ds[80] = PROJECT_NAME EXT_3DS FILE_EXT;
    char* filename = default_filename;
    char* filename_3ds = default_filename_3ds;
    int i, verbose = 0;

    for(i = 1; i < argc; ++i) {
        if(!strcmp("-v", argv[i])) {
            verbose = 1;
        }
        if(!strcmp("-f", argv[i])) {
            if(i + 1 < argc) {
                filename = argv[i + 1];
            }
        }
    }

    scene = (SCENE*)malloc(sizeof(SCENE));

    if(read_scene_file(scene, filename)) {
        printf("Il y a un probleme: je ne peux pas lire le fichier: %s\n", filename);
        return -1;
    }

    if(read_scene_file_3ds(g_scenes3DS, filename_3ds)) {
        printf("Il y a un probleme: je ne peux pas lire le fichier: %s\n", filename_3ds);
        return -1;
    }

    if(verbose) {
        print_scene_data(scene);
    }

    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);   /* taille de la fenetre ecran */

    /* glutInitDisplayMode(GLUT_INDEX | GLUT_SINGLE);   mode index*/
        
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);     /* mode rgb */
        
    glutCreateWindow("Mario Kart LO12");
    glutDisplayFunc(dessine_scene);
    glutReshapeFunc(reshape);
    glutIdleFunc(idleFunc);
    glutSpecialFunc(callSpecialFunc);
    glutKeyboardFunc(callKeyboardFunc);
    glutMouseFunc(callMouseFunc);
    glutMotionFunc(callMotionFunc);

    init();
    glutMainLoop();
    return 0;
}

