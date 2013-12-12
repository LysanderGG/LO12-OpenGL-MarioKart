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

SCENE*          scene;
<<<<<<< HEAD
Lib3dsFile*     scenes3DS[NB_MAX_3DS_SCENE];
=======
SCENE_3DS       g_scenes3DS[NB_MAX_3DS_SCENE];
>>>>>>> a00729d6d144ff66c5557e74a60e4b9603b2a163

int getTotalNbObjects() {
    int res, i;

    res = scene->nbobj;
    for(i = 0; i < NB_MAX_3DS_SCENE; ++i) {
<<<<<<< HEAD
        res += (scenes3DS[i] != NULL) ? scenes3DS[i]->nmeshes : 0;
=======
        res += (g_scenes3DS[i].lib3dsfile != NULL) ? g_scenes3DS[i].lib3dsfile->nmeshes : 0;
>>>>>>> a00729d6d144ff66c5557e74a60e4b9603b2a163
    }
    return res;
}