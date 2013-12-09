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
Lib3dsFile*     scenes3DS[NB_MAX_3DS_SCENE];

int getTotalNbObjects() {
    int res, i;

    res = scene->nbobj;
    for(i = 0; i < NB_MAX_3DS_SCENE; ++i) {
        res += (scenes3DS[i] != NULL) ? scenes3DS[i]->nmeshes : 0;
    }
    return res;
}