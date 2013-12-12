#ifndef OBJ3DS_H
#define OBJ3DS_H
#include <3DS/lib3ds.h>
<<<<<<< HEAD
=======
#include "scene.h"

#define NB_MAX_3DS_SCENE    10

typedef struct {
    Lib3dsFile* lib3dsfile;
    float       scale;
} SCENE_3DS;

extern SCENE_3DS    g_scenes3DS[NB_MAX_3DS_SCENE];    /* scenes 3DS */
>>>>>>> a00729d6d144ff66c5557e74a60e4b9603b2a163

void charge_scene3ds(char * fichier3ds, Lib3dsFile** out_scene3ds);
void dessine_scene3ds(Lib3dsFile* scene3ds);
void dessine_3dsobj(Lib3dsFile* scene3ds, Lib3dsMesh * Obj);

#endif
