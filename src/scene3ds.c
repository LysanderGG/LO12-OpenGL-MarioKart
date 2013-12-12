#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include "scene3ds.h"
#include "eclairage.h"
#include "animations.h"
#include "interactions.h"

#if defined(__APPLE__) && defined(__MACH__)
        #include <GLUT/glut.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
#else
        #include <GL/glut.h>
        #include <GL/gl.h>
        #include <GL/glu.h>
#endif

extern int g_isCurrentObject;
static int log_level = LIB3DS_LOG_INFO;

static long fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin) {
    FILE *f = (FILE*)self;
    int o;
    switch(origin) {
        case LIB3DS_SEEK_SET:
            o = SEEK_SET;
            break;
        case LIB3DS_SEEK_CUR:
            o = SEEK_CUR;
            break;
        case LIB3DS_SEEK_END:
            o = SEEK_END;
            break;
    }
    return (fseek(f, offset, o));
}

//***********************************************************************************
static long fileio_tell_func(void *self) {
    FILE *f = (FILE*)self;
    return(ftell(f));
}

//***********************************************************************************
static size_t fileio_read_func(void *self, void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fread(buffer, 1, size, f));
}

//***********************************************************************************
static size_t fileio_write_func(void *self, const void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fwrite(buffer, 1, size, f));
}

//***********************************************************************************
static void fileio_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg) {
    static const char * level_str[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    if(log_level >= level) {
        int i;
        printf("%5s : ", level_str[level]);
        for(i = 1; i < indent; ++i) {
			printf("\t");
        }
        printf("%s\n", msg);
    }
}

//*************************************************************************************
void dessineFace(Lib3dsFile* scene3ds, Lib3dsMesh * Obj, int iFace) {
    int i;
    double x, y, z, texx, texy;

    if(g_isCurrentObject) {
        def3DSSelectedMatiere(scene3ds, Obj->faces[iFace].material);
    } else {
        def3DSMatiere(scene3ds, Obj->faces[iFace].material);    
    }
    
    glBegin(GL_POLYGON);    
    for(i = 0; i < 3; ++i) {
        x = Obj->vertices[ Obj->faces[iFace].index[i] ][0];
        y = Obj->vertices[ Obj->faces[iFace].index[i] ][1];
        z = Obj->vertices[ Obj->faces[iFace].index[i] ][2];
        texx = Obj->texcos[ Obj->faces[iFace].index[i] ][0];
        texy = Obj->texcos[ Obj->faces[iFace].index[i] ][1];
        glTexCoord2d(texx, texy);
        glVertex3d(x,y,z);
    }
    glEnd();
}

//***************************************************************************************
void dessine_3dsobj(SCENE_3DS scene3ds, Lib3dsMesh * Obj) {
    int i;
    glPushMatrix();
    // Scale for each opengl object
    glScaled(scene3ds.scale, scene3ds.scale, scene3ds.scale);

    // Kart only translate and rotate
    if(g_current3DSScene == KART_ID) {
        glTranslated(g_kartX, g_kartY, g_kartZ);
        glRotated(g_kartAngle, 0, 0, 1);
        glTranslated(-g_kartX, -g_kartY, -g_kartZ);
    }

    for(i=0; i<Obj->nfaces; i++) {
        dessineFace(scene3ds.lib3dsfile, Obj, i);
    }
    glPopMatrix();
}

//***********************************************************************************
void charge_scene3ds(char * fichier3ds, Lib3dsFile** out_scene3ds) {
    FILE * file;
    Lib3dsIo io;

    *out_scene3ds = lib3ds_file_new();

    printf("LOAD 3DS SCENE FILE\n");

    if(!fichier3ds) {
        printf("[ERROR] File name NULL.\n");
        return;
    }

    if(!(file = fopen(fichier3ds, "rb"))) {
        printf("[ERROR] File not found : %s\n", fichier3ds);
        return;
    }

    io.self         = file;
    io.seek_func    = fileio_seek_func;
    io.tell_func    = fileio_tell_func;
    io.read_func    = fileio_read_func;
    io.write_func   = fileio_write_func;
    io.log_func     = fileio_log_func;
    
    lib3ds_file_read(*out_scene3ds, &io);

    printf("LOAD 3DS SCENE FILE - END\n");
}

//**************************************************************************
void dessine_scene3ds(SCENE_3DS scene3ds) {
    int i;
    glScaled(scene3ds.scale, scene3ds.scale, scene3ds.scale);
    for (i = 0; i < scene3ds.lib3dsfile->nmeshes; ++i) {
        dessine_3dsobj(scene3ds, scene3ds.lib3dsfile->meshes[i]);
    }
}