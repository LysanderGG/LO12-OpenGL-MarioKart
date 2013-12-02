#ifndef ECLAIRAGE_H
#define ECLAIRAGE_H

#include "scene.h"

void calcule_normales(SCENE *scene);
void def_sources(SCENE *scene);
void def_modele(SCENE *scene);
void def_matiere(SCENE *scene, int i);
void def_selectedMatiere(SCENE *scene, int i);

#endif

