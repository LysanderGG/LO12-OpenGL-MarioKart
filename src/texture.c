#include <stdlib.h>
#include <stdio.h>

#include "scene3ds.h"
#include "pngloader.h"
#include "texture.h"
#include "Utils.h"

extern SCENE_3DS       g_scenes3DS[NB_MAX_3DS_SCENES];
extern int             g_nbScenes3DS;
extern int             g_nbRandomObjects;

int loadTextures() {
    int i = 0;
    GLuint* tabIndTex = (GLuint*) malloc(sizeof(GLuint) * scene->nbtex);

    glGenTextures(scene->nbtex, tabIndTex);

    for(i = 0; i < scene->nbtex; ++i) {
        if(ReadPNGFromFile(&(scene->tabtex[i]))) {
            printf("Impossible de charger %s", scene->tabtex[i].filename);
            return 0;
        }
        scene->tabtex[i].glnum = tabIndTex[i];

        glBindTexture(GL_TEXTURE_2D, scene->tabtex[i].glnum);
        //Paramètres de gestion des textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // Paramètres pour charger la texture
        
        glTexImage2D (GL_TEXTURE_2D, 0, scene->tabtex[i].internalFormat,
                      scene->tabtex[i].width, scene->tabtex[i].height, 0, scene->tabtex[i].format,
                      GL_UNSIGNED_BYTE, scene->tabtex[i].texels);
    }

    return 1;
}

int load3dsTextures() {
    MTEX* texture = (MTEX *)malloc(1*sizeof(MTEX));
    int iScene;
    int iMat;

    for(iScene = 0; iScene < g_nbScenes3DS; ++iScene) {
        for(iMat = 0; iMat < g_scenes3DS[iScene].lib3dsfile->nmaterials; ++iMat) {
            //only one texture1_map for the scene can be loaded
            if(strlen(g_scenes3DS[iScene].lib3dsfile->materials[iMat]->texture1_map.name) != 0 
                && g_scenes3DS[iScene].texture[0] == 0) {

                texture->filename = concat(PATH_TEXTURES,g_scenes3DS[iScene].lib3dsfile->materials[iMat]->texture1_map.name);
                if(ReadPNGFromFile(texture)) {
                    printf("Impossible de charger %s", texture->filename);
                    return 0;
                }

                glGenTextures(1, g_scenes3DS[iScene].texture);
                glBindTexture(GL_TEXTURE_2D, g_scenes3DS[iScene].texture[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexImage2D(GL_TEXTURE_2D, 0, texture[0].internalFormat,
                              texture[0].width, texture[0].height, 0, texture[0].format,
                              GL_UNSIGNED_BYTE, texture[0].texels);
                break;
            }
        }
    }
    return 1;
}