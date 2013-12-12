/*
 
        Universite Technologique de Compiegne
        
        UV: LO12
        
        FICHIER: scene.c
 
        COMMENTAIRE:
                variables globales
 
        AUTEURS:
                Veronique BERGE-CHERFAOUI
                DG
                Olivier BEZET  A2002-A2005
                Romain HERAULT A2005-A2006
*/

#include "scene.h"
#include "scene3ds.h"

SCENE*          scene;
SCENE_3DS       g_scenes3DS[NB_MAX_3DS_SCENE];

int getTotalNbObjects() {
    int res, i;

    res = scene->nbobj;
    for(i = 0; i < NB_MAX_3DS_SCENE; ++i) {
        res += (g_scenes3DS[i].lib3dsfile != NULL) ? g_scenes3DS[i].lib3dsfile->nmeshes : 0;
    }
    return res;
}