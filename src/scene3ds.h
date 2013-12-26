#ifndef OBJ3DS_H
#define OBJ3DS_H
#include <3DS/lib3ds.h>
#include "scene.h"

typedef struct {
    Lib3dsFile* lib3dsfile;
    GLuint      lights[2];
    float       translate[3];
    float       rotate[3];
    float       scale;
} SCENE_3DS;

extern SCENE_3DS    g_scenes3DS[NB_MAX_3DS_SCENES];    /* scenes 3DS */
extern int          g_nbScenes3DS;

int charge_scene3ds(char * fichier3ds, Lib3dsFile** out_scene3ds);
void dessine_scene3ds(SCENE_3DS scene3ds);
void dessine_3dsobj(SCENE_3DS scene3ds, Lib3dsMesh* Obj);

#endif
