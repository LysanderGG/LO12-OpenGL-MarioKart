#ifndef ECLAIRAGE_H
#define ECLAIRAGE_H

#include "scene.h"

#define SELECTED_OBJECT_COLOR_R 0.4f
#define SELECTED_OBJECT_COLOR_G 0.0f
#define SELECTED_OBJECT_COLOR_B 0.0f
#define SELECTED_OBJECT_COLOR_A 0.4f

void calcule_normales(SCENE *scene);
void def_sources(SCENE *scene);
void def_modele(SCENE *scene);
void def_matiere(SCENE *scene, int i);
void def_selectedMatiere(SCENE *scene, int i);
void def3DSMatiere(Lib3dsFile* scene3ds, int i);
void def3DSSelectedMatiere(Lib3dsFile* scene3ds, int i);

#endif

