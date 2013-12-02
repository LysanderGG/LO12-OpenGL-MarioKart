#ifndef AFFICHE_H
#define AFFICHE_H

#include "scene.h"

void dessine_face(INDICE_FACE iface, int _differentColorForEachVertex);
void dessine_objet(OBJET objet);
void dessine_scene();
void dessine_repere();
void reshape(int _w, int _h);
void idleFunc();

#endif

