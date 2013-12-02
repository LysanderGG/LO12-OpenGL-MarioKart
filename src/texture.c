#include <stdlib.h>
#include <stdio.h>

#include "texture.h"
#include "pngloader.h"

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
