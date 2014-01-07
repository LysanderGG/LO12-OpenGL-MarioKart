#ifndef SCENE_UTILS_H
#define SCENE_UTILS_H

#include "scene.h"
#include "scene3ds.h"

int read_scene_file(SCENE *scene,char *filename);
int read_scene_file_3ds(char *filename);
int load_3ds_models_and_randomize(char* filename);
void reload_random_objects(char* filename);

void print_scene_data(SCENE *scene);

#endif
