#ifndef OBJ3DS_H
#define OBJ3DS_H
#include <3DS/lib3ds.h>

void charge_scene3ds(char * fichier3ds, Lib3dsFile** out_scene3ds);
void dessine_scene3ds(Lib3dsFile* scene3ds);
void dessine_3dsobj(Lib3dsFile* scene3ds, Lib3dsMesh * Obj);

#endif
