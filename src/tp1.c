#ifdef WIN32
    #pragma warning( disable : 4996 )
#endif

//******************//
//Fichiers inclus : //
//******************//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shadow.h"

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
#include "observateur.h"
#include "scene3ds.h"
#include "skybox.h"
#include "scene.h"
#include "SceneUtils.h"
#include "texture.h"

//**************//
//DEFINITIONS : //
//**************//

#define PROJECT_NAME "scene"
#define EXT_3DS "_3ds"
#define FILE_EXT ".dat"
#define MODELE_FILE_NAME "random_models.dat"


/*******************************************************************************/
/*                      Initialisations pour GL et Glut                        */
/*******************************************************************************/
int init() {
    // Must be first
    initView();

    if(initShadow() == -1) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST | GL_LIGHTING);

    /* mode RGB : choix de la couleur de fond */
    glClearColor(0,0,0,1);

    /* applique la couleur de fond */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    def_modele(scene);
    //animationInit(1);

    loadTextures();
    load3dsTextures();
    charger_skybox(DAY);

    redefineLights();

    return 0;
}

/* ========================================================================= */

int main(int argc, char**argv) {
    char  default_filename[80]          = PROJECT_NAME FILE_EXT;
    char  default_filename_3ds[80]      = PROJECT_NAME EXT_3DS FILE_EXT;
    char  default_filename_models[80]   = MODELE_FILE_NAME;
    char* filename          = default_filename;
    char* filename_3ds      = default_filename_3ds;
    char* filename_models   = default_filename_models;
    int i, verbose = 0;
    int nb3DSRandomObjects;

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

    if(read_scene_file_3ds(g_scenes3DS, filename_3ds, &g_nbScenes3DS)) {
        printf("Il y a un probleme: je ne peux pas lire le fichier: %s\n", filename_3ds);
        return -1;
    }

    for(i = 0; i < g_nbScenes3DS; ++i) {
        printf("3DS Scene\n");
        printf("Scene #%d\n", i);
        printf("  libs3dsfile: %p\n", g_scenes3DS[i].lib3dsfile);
        printf("  lights   : %x %x\n", g_scenes3DS[i].lights[0], g_scenes3DS[i].lights[1]);
        printf("  Scale    : %4.2f\n", g_scenes3DS[i].scale);
        printf("  Translate: %4.2f %4.2f %4.2f\n", g_scenes3DS[i].translate[0], g_scenes3DS[i].translate[1], g_scenes3DS[i].translate[2]);
        printf("  Rotate   : %4.2f %4.2f %4.2f\n", g_scenes3DS[i].rotate[0], g_scenes3DS[i].rotate[1], g_scenes3DS[i].rotate[2]);
    }
    if(load_3ds_models_and_randomize(g_scenes3DS, filename_models, &nb3DSRandomObjects)) {
        printf("Il y a un probleme: je ne peux pas lire le fichier: %s\n", filename_models);
        return -1;
    }

    // Add Random 3DS objects to the number of scenes.
    g_nbScenes3DS += nb3DSRandomObjects;

    for(i = 0; i < g_nbScenes3DS; ++i) {
        printf("3DS Scene\n");
        printf("Scene #%d\n", i);
        printf("  libs3dsfile: %p\n", g_scenes3DS[i].lib3dsfile);
        printf("  lights   : %x %x\n", g_scenes3DS[i].lights[0], g_scenes3DS[i].lights[1]);
        printf("  Scale    : %4.2f\n", g_scenes3DS[i].scale);
        printf("  Translate: %4.2f %4.2f %4.2f\n", g_scenes3DS[i].translate[0], g_scenes3DS[i].translate[1], g_scenes3DS[i].translate[2]);
        printf("  Rotate   : %4.2f %4.2f %4.2f\n", g_scenes3DS[i].rotate[0], g_scenes3DS[i].rotate[1], g_scenes3DS[i].rotate[2]);
    }

    if(verbose) {
        print_scene_data(scene);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);     /* mode rgb */
    glutInitWindowSize(800, 600);   /* taille de la fenetre ecran */
    glutCreateWindow("Mario Kart LO12");

    if(init() == -1) {
        return -1;
    }

    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(idleFunc);
    glutSpecialFunc(callSpecialFunc);
    glutKeyboardFunc(callKeyboardDownFunc);
    glutKeyboardUpFunc(callKeyboardUpFunc);
    glutMouseFunc(callMouseFunc);
    glutMotionFunc(callMotionFunc);

    //glutTimerFunc(10, animationTimer, 0);
    glutMainLoop();
    return 0;
}

