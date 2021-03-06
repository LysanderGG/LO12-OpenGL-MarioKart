#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

#include "GLee/GLee.h" //GL header file, including extensions
#if defined(__APPLE__) && defined(__MACH__)
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "eclairage.h"
#include "scene.h"
#include "shadow.h"
#include "Utils.h"

extern SCENE* scene;


GLuint g_nextLight      = GL_LIGHT0;
unsigned int g_nbLights = 0;
SOURCE g_lights[MAX_LIGHTS];


// Prototypes
void calcule_normale(SCENE *scene, int i);


/*******************************************************************************/
/*                         Mise en place de l'eclairage                        */
/*******************************************************************************/

void calcule_normales(SCENE *scene) {
    int i = 0;
    for(i = 0; i < scene->nbface; ++i) {
        calcule_normale(scene, i);
    }
}

void calcule_normale(SCENE *scene, int i) {
    MCOORD v1, v2;
    
    calcule_vecteur( scene->tabpt[scene->tabface[i].tabpt[0]]
                   , scene->tabpt[scene->tabface[i].tabpt[1]]
                   , &v1);
    calcule_vecteur( scene->tabpt[scene->tabface[i].tabpt[0]]
                   , scene->tabpt[scene->tabface[i].tabpt[scene->tabface[i].nbpt-1]]
                   , &v2);
    prod_vectoriel(v1, v2, &scene->tabface[i].n);

    normalise(&scene->tabface[i].n);
}

void rgb2rgbaf(RGB *rgb, GLfloat alpha, RGBAF *rgbaf) {
    rgbaf->r = rgb->r;
    rgbaf->g = rgb->g;
    rgbaf->b = rgb->b;
    rgbaf->a = alpha;
}

void mcoord2mcoordf(MCOORD *mcoord,MCOORDF *mcoordf) {
    mcoordf->x = mcoord->x;
    mcoordf->y = mcoord->y;
    mcoordf->z = mcoord->z;
    mcoordf->w = mcoord->w;
}

/*****************************************************************************/
void def_sources(SCENE *scene) {
    RGBAF propc;    /* tableau des proprietes de la source */
    MCOORDF propp;
    GLenum source;
    int i = 0;
    
    for(i = 0; i < scene->nbsource; ++i) {
        source = g_nextLight;
        ++g_nextLight;

        // Save datas in global variable
        g_lights[g_nbLights].allume     = scene->tabsource[i].allume;
        g_lights[g_nbLights].allure_faisceau = scene->tabsource[i].allure_faisceau;
        g_lights[g_nbLights].direction  = scene->tabsource[i].direction;
        g_lights[g_nbLights].position   = scene->tabsource[i].position;

        if(scene->tabsource[i].allume) {
            rgb2rgbaf(&scene->tabsource[i].ambiante, 1, &propc);
            glLightfv(source,GL_AMBIENT,propc.rgba);

            rgb2rgbaf(&scene->tabsource[i].couleur, 1, &propc);
            glLightfv(source,GL_DIFFUSE,propc.rgba);
            glLightfv(source,GL_SPECULAR,propc.rgba);

            mcoord2mcoordf(&scene->tabsource[i].position, &propp);
            glLightfv(source,GL_POSITION,propp.vect);

            glLightf(source,GL_SPOT_EXPONENT, scene->tabsource[i].allure_faisceau.k);
            glLightf(source,GL_SPOT_CUTOFF, scene->tabsource[i].allure_faisceau.theta);

            mcoord2mcoordf(&scene->tabsource[i].direction, &propp);
            glLightfv(source,GL_SPOT_DIRECTION,propp.vect);

            glEnable(source);
        } else {
            glDisable(source);
        }

        ++g_nbLights;
    }
}

/*****************************************************************************/
void def_modele(SCENE *scene) {
    RGBAF propc;

    if(!scene) {
        return;
    }

    propc.r = 0.2;
    propc.g = 0.2;
    propc.b = 0.2;
    propc.a = 1.0;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,propc.rgba);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

/*****************************************************************************/
void def_matiere(SCENE *scene, int i) {
    RGBAF propc;
    INDICE_TEX texIndex;

    // Couleur
    rgb2rgbaf(&scene->tabmat[i].ambiante, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, propc.rgba);

    rgb2rgbaf(&scene->tabmat[i].diffuse, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, propc.rgba);

    rgb2rgbaf(&scene->tabmat[i].speculaire, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, propc.rgba);

    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, scene->tabmat[i].brillance);

    rgb2rgbaf(&scene->tabmat[i].emission, 1, &propc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, propc.rgba);

    // Texture
    texIndex = scene->tabmat[i].tex;
    glBindTexture(GL_TEXTURE_2D, scene->tabtex[texIndex].glnum);
}

void def_selectedMatiere(SCENE *scene, int i) {
    INDICE_TEX texIndex;
    RGBAF propc;

    // Couleur
    propc.r = SELECTED_OBJECT_COLOR_R;
    propc.g = SELECTED_OBJECT_COLOR_G;
    propc.b = SELECTED_OBJECT_COLOR_B;
    propc.a = SELECTED_OBJECT_COLOR_A;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , propc.rgba);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, scene->tabmat[i].brillance);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION , propc.rgba);

    // Texture
    texIndex = scene->tabmat[i].tex;
    glBindTexture(GL_TEXTURE_2D, scene->tabtex[texIndex].glnum);
}

//////////////////////////////////////////////////////////////////////////////////////
void def3DSSources(SCENE_3DS* scene3ds) {
    Lib3dsLight* light;
    GLfloat diff[4];
    GLfloat amb[4];
    GLfloat pos[4];
    GLfloat tar[4];
    GLfloat attenuation[1];
    GLfloat fallOff[1];
    float angle;
    int j;
    int iScene;
    int iLight;
    float v[3];
    float newX, newY;


    for(iScene = 0; iScene < g_nbScenes3DS && scene3ds[iScene].lib3dsfile != NULL; ++iScene) {
        for(iLight = 0; iLight < scene3ds[iScene].lib3dsfile->nlights; ++iLight) {
            light = scene3ds[iScene].lib3dsfile->lights[iLight];
            scene3ds[iScene].lights[iLight] = g_nextLight;

	        for(j = 0; j < 3; ++j) {
		        diff[j] = light->color[j];
		        amb[j] = light->color[j] / 4.5;
                // Vector from the kart center to the light source point.
                v[j] = light->position[j] * scene3ds[iScene].scale;
	        }
	        diff[3] = amb[3] = 1.0;

            angle = (scene3ds[iScene].rotate[2] - scene3ds->rotateAnimationInit[2] + scene3ds->rotateAnimation[2]) * M_PI / 180.0f;

             // Vector coordinates afer rotation
            newX = v[0] * cos(angle) - v[1] * sin(angle);
            newY = v[0] * sin(angle) + v[1] * cos(angle);

            // Light source point position
            pos[0] = newX + scene3ds[iScene].translate[0] + (-scene3ds[iScene].translateAnimationInit[0] + scene3ds[iScene].translateAnimation[0]) * scene3ds->scale;
            pos[1] = newY + scene3ds[iScene].translate[1] + (-scene3ds[iScene].translateAnimationInit[1] + scene3ds[iScene].translateAnimation[1]) * scene3ds->scale;
            pos[2] = v[2] + scene3ds[iScene].translate[2] + (-scene3ds[iScene].translateAnimationInit[2] + scene3ds[iScene].translateAnimation[2]) * scene3ds->scale;
            pos[3] = 1.0;

            glLightfv(g_nextLight, GL_DIFFUSE, diff);
            glLightfv(g_nextLight, GL_SPECULAR, diff);
	        glLightfv(g_nextLight, GL_POSITION, pos);
	        glLightfv(g_nextLight, GL_AMBIENT, amb);

            // Save datas in global variable
            g_lights[g_nbLights].allume     = (light->off + 1) % 2;
            g_lights[g_nbLights].position.x = pos[0];
            g_lights[g_nbLights].position.y = pos[1];
            g_lights[g_nbLights].position.z = pos[2];
            g_lights[g_nbLights].position.w = pos[3];

            if(light->spot_light) {
                tar[0] = cos(angle);
                tar[1] = sin(angle);
                tar[2] = 0.0f;
                tar[3] = 1.0f;

                glLightfv(g_nextLight, GL_SPOT_DIRECTION, tar);
                fallOff[0] = light->falloff;
                glLightfv(g_nextLight, GL_SPOT_CUTOFF, fallOff);
                attenuation[0] = 0.0;
                if(light->attenuation > 50.0) {
                    attenuation[0] = 50.0;
                } else {
                    attenuation[0] =  light->attenuation;
                }

                glLightfv(g_nextLight, GL_LINEAR_ATTENUATION, attenuation);

                // Save data in global variable
                g_lights[g_nbLights].direction.x = tar[0];
                g_lights[g_nbLights].direction.y = tar[1];
                g_lights[g_nbLights].direction.z = tar[2];
                g_lights[g_nbLights].direction.w = tar[3];
                g_lights[g_nbLights].allure_faisceau.k     = 0.0;
                g_lights[g_nbLights].allure_faisceau.theta = fallOff[0];
	        }
            ++g_nextLight;
            ++g_nbLights;
        }
    }

}

void def3DSMatiere(SCENE_3DS* scene3ds, int i) {
    Lib3dsMaterial* mat;

    mat = scene3ds->lib3dsfile->materials[i];
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess);
}

void def3DSSelectedMatiere(Lib3dsFile* scene3ds, int i) {
    RGBAF propc;
    propc.r = SELECTED_OBJECT_COLOR_R;
    propc.g = SELECTED_OBJECT_COLOR_G;
    propc.b = SELECTED_OBJECT_COLOR_B;
    propc.a = SELECTED_OBJECT_COLOR_A;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, propc.rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, propc.rgba);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, scene3ds->materials[i]->shininess);
}

void switchLight(GLuint light) {
    printf("switch light %d\n", light - GL_LIGHT0);
    if(glIsEnabled(light)) {
        glDisable(light);
    } else {
        glEnable(light);
    }
}

void turnOnLight(GLuint light) {
    glEnable(light);
}

void turnOffLight(GLuint light) {
    glDisable(light);
}

void drawLights(SCENE* scene) {
    int i;
    
    for(i = 0; i < scene->nbsource; ++i) {
        glDisable(GL_LIGHTING);
        glBegin(GL_POINT);
        glColor3f(1.0f, .0f, .0f);
        glVertex3f(scene->tabsource[i].position.x,
                   scene->tabsource[i].position.y,
                   scene->tabsource[i].position.z);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(scene->tabsource[i].position.x,
                   scene->tabsource[i].position.y,
                   scene->tabsource[i].position.z);

        glVertex3f(scene->tabsource[i].position.x + scene->tabsource[i].direction.x * 5.0,
                   scene->tabsource[i].position.y + scene->tabsource[i].direction.y * 5.0,
                   scene->tabsource[i].position.z + scene->tabsource[i].direction.z * 5.0);
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

void draw3DSLights(SCENE_3DS* scene3ds) {
    Lib3dsLight* light;
    GLfloat pos[4];
    GLfloat tar[4];
    float angle;
    int j;
    int iScene;
    int iLight;
    float newX, newY;
    float v[3];

    for(iScene = 0; iScene < g_nbScenes3DS && scene3ds[iScene].lib3dsfile != NULL; ++iScene) {
        for(iLight = 0; iLight < scene3ds[iScene].lib3dsfile->nlights; ++iLight) {
            light = scene3ds[iScene].lib3dsfile->lights[iLight];

            // Vector from the kart center to the light source point.
            for(j = 0; j < 3; ++j) {
                v[j] = light->position[j] * scene3ds[iScene].scale;
	        }

            if(light->spot_light) {
                angle = scene3ds[iScene].rotate[2] * M_PI / 180.0f;
                tar[0] = cos(angle);
                tar[1] = sin(angle);
                tar[2] = 0.0f;
                tar[3] = 1.0f;

                // Vector coordinates afer rotation
                newX = v[0] * cos(angle) - v[1] * sin(angle);
                newY = v[0] * sin(angle) + v[1] * cos(angle);

                // Light source point position
                pos[0] = newX + scene3ds[iScene].translate[0];
                pos[1] = newY + scene3ds[iScene].translate[1];
                pos[2] = v[2] + scene3ds[iScene].translate[2];
                pos[3] = 1.0;

                glDisable(GL_LIGHTING);
                glBegin(GL_LINES);
                glColor3f(0.0f, 1.0f, 1.0f);
                glVertex3f(pos[0], pos[1], pos[2]);
                glVertex3f(pos[0] + tar[0] * 5, pos[1] + tar[1] * 5, pos[2] + tar[2] * 5);
                glEnd();
                glEnable(GL_LIGHTING);
	        }
        }
    }
}
