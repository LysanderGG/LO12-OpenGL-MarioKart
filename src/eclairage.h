#ifndef ECLAIRAGE_H
#define ECLAIRAGE_H

#include "scene.h"
#include "scene3ds.h"

#define SELECTED_OBJECT_COLOR_R 0.4f
#define SELECTED_OBJECT_COLOR_G 0.0f
#define SELECTED_OBJECT_COLOR_B 0.0f
#define SELECTED_OBJECT_COLOR_A 0.4f
#define MAX_LIGHTS              10

void def_sources(SCENE *scene);
void def_modele(SCENE *scene);
void def_matiere(SCENE *scene, int i);
void def_selectedMatiere(SCENE *scene, int i);

void def3DSSources(SCENE_3DS* scene3ds);
void def3DSMatiere(SCENE_3DS* scene3ds, int i);
void def3DSSelectedMatiere(Lib3dsFile* scene3ds, int i);

void switchLight(GLuint light);
void turnOnLight(GLuint light);
void turnOffLight(GLuint light);

void draw3DSLights(SCENE_3DS* scene3ds);

// Light Management global variables
extern GLuint g_nextLight;
extern unsigned int g_nbLights;
extern SOURCE g_lights[MAX_LIGHTS];

#endif
