#ifndef SCENE_UTILS_H
#define SCENE_UTILS_H

#include "scene.h"
#include "scene3ds.h"

int read_scene_file(SCENE *scene,char *filename);
int read_scene_file_3ds(SCENE_3DS* scene,char *filename);
void print_scene_data(SCENE *scene);

#endif
