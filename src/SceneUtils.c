/*
 
    Universite Technologique de Compiegne
        
    UV: LO12
        
    FICHIER: SceneUtils.c
 
    COMMENTAIRE:
            Lecture du fichier Scene (Haut Niveau)
 
    AUTEURS:
            Veronique BERGE-CHERFAOUI
            DG
            Olivier BEZET  A2002-A2005
            Romain HERAULT A2005-A2006
*/

#ifdef WIN32
    #pragma warning( disable : 4996 )
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "DatUtils.h"
#include "scene.h"
#include "SceneUtils.h"
#include "texture.h"

#ifdef _MSC_VER
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

extern SCENE *scene;

/**************************************************************************************/
/* lecture du contenu de la scene dans un fichier et stockage dans la structure scene */
/**************************************************************************************/

/*Lit #num int de la ligne courante et le stocke dans le tableau d'int value*/
int read_int(FILE *file,int *line,char *str,int num, int *value)
{
    int i;
    ValueList *vlf,*vl;
    vlf=vl=readNextLine(file,line);
    if(vl==NULL) {
        printf("ERREUR ligne %d lecture de %s, impossible de lire le fichier\n",*line,str);
        return -1;
    }
    for(i=0;i<num && vl;i++,vl=vl->next) {
        if(vl->type!=INT_VALUE) {
            printf("ERREUR ligne %d lecture de %s, elements %d, type incompatible\n",*line,str,i);
            printf("J'attends un INT et je lis: ");
            printValue(vl);
            printf("\n");
            freeValueList(vlf);
            return -1;
        }
        value[i]=vl->intValue;
    }
    if(i!=num) {
        printf("ERREUR ligne %d lecture de %s, trop peu d'elements\n",*line,str);
        printf("J'attends %d INT et je lis: ",num);
        printValueList(vlf);
        freeValueList(vlf);
        return -1;
    }
    if(vl!=NULL) {
        printf("ATTENTION ligne %d lecture de %s, trop d'elements\n",*line,str);
        printf("J'attends %d INT et je lis: ",num);
        printValueList(vlf);
    }
    freeValueList(vlf);
    return 0;
}

/*Lit #num double de la ligne courante et le stocke dans le tableau de double value*/
int read_double(FILE *file,int *line,char *str,int num, double *value)
{
    int i;
    ValueList *vlf,*vl;
    vlf=vl=readNextLine(file,line);
    if(vl==NULL) {
        printf("ERREUR ligne %d lecture de %s, impossible de lire le fichier\n",*line,str);
        return -1;
    }
    for(i=0;i<num && vl;i++,vl=vl->next) {
        if(vl->type!=DOUBLE_VALUE) {
            printf("ERREUR ligne %d lecture de %s, elements %d, type incompatible\n",*line,str,i);
            printf("J'attends un DOUBLE et je lis: ");
            printValue(vl);
            printf("\n");
            freeValueList(vlf);
            return -1;
        }
        value[i]=vl->doubleValue;
    }
    if(i!=num) {
        printf("ERREUR ligne %d lecture de %s, trop peu d'elements\n",*line,str);
        printf("J'attends %d DOUBLE et je lis: ",num);
        printValueList(vlf);
        freeValueList(vlf);
        return -1;
    }
    if(vl!=NULL) {
        printf("ATTENTION ligne %d lecture de %s, trop d'elements\n",*line,str);
        printf("J'attends %d DOUBLE et je lis: ",num);
        printValueList(vlf);
    }
    freeValueList(vlf);
    return 0;
}

/*Lit #num str de la ligne courante et le stocke dans le tableau de char * pstr */
int read_str(FILE *file,int *line,char *str,int num, char **pstr)
{
    int i;
    ValueList *vlf,*vl;
    vlf=vl=readNextLine(file,line);
    if(vl==NULL) {
        printf("ERREUR ligne %d lecture de %s, impossible de lire le fichier\n",*line,str);
        return -1;
    }
    for(i=0;i<num && vl;i++,vl=vl->next) {
        if(vl->type!=STR_VALUE) {
            printf("ERREUR ligne %d lecture de %s, elements %d, type incompatible\n",*line,str,i);
            printf("J'attends un STR et je lis: ");
            printValue(vl);
            printf("\n");
            freeValueList(vlf);
            return -1;
        }
        pstr[i]=(char*)malloc(strlen(vl->strValue)+1);
        strcpy(pstr[i],vl->strValue);
    }
    if(i!=num) {
        printf("ERREUR ligne %d lecture de %s, trop peu d'elements\n",*line,str);
        printf("J'attends %d STR et je lis: ",num);
        printValueList(vlf);
        freeValueList(vlf);
        return -1;
    }
    if(vl!=NULL) {
        printf("ATTENTION ligne %d lecture de %s, trop d'elements\n",*line,str);
        printf("J'attends %d STR et je lis: ",num);
        printValueList(vlf);
    }
    freeValueList(vlf);
    return 0;
}

/*Lit un MTEXCOORD (en 2 double) dans la ligne courante et le stocke dans coord*/
int read_texcoord2d(FILE *file,int *line,char *str,MTEXCOORD *coord)
{
    double buff[2];
    if(read_double(file,line,str,2,buff))
            return -1;
    coord->x=buff[0];
    coord->y=buff[1];
    return 0;
}

/*Lit un MCOORD (en 3 double) dans la ligne courante et le stocke dans coord*/
int read_coord3d(FILE *file,int *line,char *str,MCOORD *coord)
{
    double buff[3];
    if(read_double(file,line,str,3,buff))
        return -1;
    coord->x=buff[0];
    coord->y=buff[1];
    coord->z=buff[2];
    coord->w=1;
    return 0;
}

/*Lit un MCOORD (en 4 double) dans la ligne courante et le stocke dans coord*/
int read_coord4d(FILE *file,int *line,char *str,MCOORD *coord)
{
    double buff[4];
    if(read_double(file,line,str,4,buff))
        return -1;
    coord->x=buff[0];
    coord->y=buff[1];
    coord->z=buff[2];
    coord->w=buff[3];
    return 0;
}

/*Lit un RGB (en 3 double) dans la ligne courante et le stocke dans rgb  */
int read_rgb3d(FILE *file,int *line,char *str,RGB *rgb)
{
    double buff[3];
    if(read_double(file,line,str,3,buff))
        return -1;
    rgb->r=buff[0];
    rgb->g=buff[1];
    rgb->b=buff[2];
    return 0;
}

/*Lit un MTRANSFO (en 4*4 double) à partir de la ligne courante et le stocke dans transfo  */
int read_transfo4x4d(FILE *file,int *line,char *str,MTRANSFO transfo)
{
    double buff[4];
    if(read_double(file,line,str,4,buff))
        return -1;
    transfo[0]=buff[0];
    transfo[4]=buff[1];
    transfo[8]=buff[2];
    transfo[12]=buff[3];
    if(read_double(file,line,str,4,buff))
        return -1;
    transfo[1]=buff[0];
    transfo[5]=buff[1];
    transfo[9]=buff[2];
    transfo[13]=buff[3];
    if(read_double(file,line,str,4,buff))
        return -1;
    transfo[2]=buff[0];
    transfo[6]=buff[1];
    transfo[10]=buff[2];
    transfo[14]=buff[3];
    if(read_double(file,line,str,4,buff))
        return -1;
    transfo[3]=buff[0];
    transfo[7]=buff[1];
    transfo[11]=buff[2];
    transfo[15]=buff[3];
    return 0;
}

/*Lit un ALLURE_FAISCEAU  (en 2 double) dans la ligne courante et le stocke dans allure  */
int read_allure2d(FILE *file, int *line, char *str, ALLURE_FAISCEAU *allure)
{
    double buff[2];
    if(read_double(file,line,str,2,buff))
        return -1;
    allure->k=buff[0];
    allure->theta=buff[1];
    return 0;
}

int read_scene_file(SCENE *scene,char *filename)
{
    FILE *file;                        /* identificateur du fichier */
    int   i, line=0;
    char signe[256];


    /*-----------------------------------------------------------------------*/

    file = fopen(filename, "r");

    if(!file) {
        fprintf(stderr, "\007\nProbleme a l'ouverture du fichier %s\n", filename);
        return(-1);
    }

    if(read_int(file,&line,"Nombre de textures",1,&(scene->nbtex) ))
        return -1;
    if(read_int(file,&line,"Nombre de coordonées de textures",1,&(scene->nbtc) ))
        return -1;
    if(read_int(file,&line,"Nombre de points",1,&(scene->nbpt) ))
        return -1;
    if(read_int(file,&line,"Nombre de matieres",1,&(scene->nbmat)))
        return -1;
    if(read_int(file,&line,"Nombre de faces",1,&(scene->nbface) ))
        return -1;
    if(read_int(file,&line,"Nombre d'objets",1,&(scene->nbobj)))
        return -1;
    if(read_int(file,&line,"Nombre de sources",1,&(scene->nbsource)) )
        return -1;

    scene->tabtex = (MTEX *)malloc(scene->nbtex * sizeof(MTEX));

    for (i = 0 ; i < scene->nbtex ; i++)    /* boucle sur les points */
    {
        SNPRINTF(signe,255,"Fichier Texture %d",i);
        if(read_str(file,&line,signe,1, &(scene->tabtex[i].filename)))
            return -1;
    }

    scene->tabtc = (MTEXCOORD *)malloc(scene->nbtc * sizeof(MTEXCOORD));

    for (i = 0 ; i < scene->nbtc ; i++)    /* boucle sur les points */
    {
        SNPRINTF(signe,255,"Coordonées de texture %d",i);
        if(read_texcoord2d(file,&line,signe,&(scene->tabtc[i])))
                return -1;
    }




    scene->tabpt = (MCOORD *)malloc(scene->nbpt * sizeof(MCOORD));

    for (i = 0 ; i < scene->nbpt ; i++)    /* boucle sur les points */
    {
        SNPRINTF(signe,255,"Point %d",i);
        if(read_coord3d(file,&line,signe,&(scene->tabpt[i])))
            return -1;
    }

    scene->tabmat = (MATIERE *)malloc( scene->nbmat * sizeof(MATIERE));

    for (i = 0 ; i < scene->nbmat ; i++)    /* boucle sur les matieres */
    {
        SNPRINTF(signe,255,"Matiere %d (alpha)",i);
        if(read_double(file,&line,signe,1,&(scene->tabmat[i].alpha)) )
            return -1;
        SNPRINTF(signe,255,"Matiere %d (ambiante)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabmat[i].ambiante)))
            return -1;
        SNPRINTF(signe,255,"Matiere %d (diffuse)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabmat[i].diffuse)))
            return -1;
        SNPRINTF(signe,255,"Matiere %d (emission)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabmat[i].emission)))
            return -1;
        SNPRINTF(signe,255,"Matiere %d (speculaire)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabmat[i].speculaire)))
            return -1;
        SNPRINTF(signe,255,"Matiere %d (brillance)",i);
        if(read_double(file,&line,signe,1,&(scene->tabmat[i].brillance)))
            return -1;
        SNPRINTF(signe,255,"Matiere %d (texture)",i);
        if(read_int(file,&line,signe,1,&(scene->tabmat[i].tex)))
            return -1;
    }



    scene->tabface = (FACE *)malloc(scene->nbface * sizeof(FACE));

    for (i = 0 ; i < scene->nbface ; i++)    /* boucle sur les faces */
    {
        SNPRINTF(signe,255,"Face %d (nombre de points)",i);
        if(read_int(file,&line,signe,1,&(scene->tabface[i].nbpt)))
            return -1;
        /* on reserve de la place pour scene->tabface[i].nbpt  INDICE_PT */
        scene->tabface[i].tabpt =
            (INDICE_PT *)malloc(scene->tabface[i].nbpt * sizeof(INDICE_PT));
        SNPRINTF(signe,255,"Face %d (liste de points)",i);
        if(read_int(file,&line,signe,scene->tabface[i].nbpt,scene->tabface[i].tabpt) )
            return -1;
        SNPRINTF(signe,255,"Face %d (indice matiere)",i);
        if(read_int(file,&line,signe,1,&(scene->tabface[i].imat)) )
            return -1;

        SNPRINTF(signe,255,"Face %d (nombre de coordonées de texture)",i);
        if(read_int(file,&line,signe,1,&(scene->tabface[i].nbtc)))
            return -1;
        /* on reserve de la place pour scene->tabface[i].nbpt  INDICE_PT */
        if(scene->tabface[i].nbtc>0)
        {
            scene->tabface[i].tabtc =
                (INDICE_TEXCOORD *)malloc(scene->tabface[i].nbtc * sizeof(INDICE_TEXCOORD));
            SNPRINTF(signe,255,"Face %d (liste de coordonées de texture)",i);
            if(read_int(file,&line,signe,scene->tabface[i].nbtc,scene->tabface[i].tabtc) )
                return -1;
        }
    }

    /* ------------------------------------------------------------------------- */

    /* on reserve de la place pour un tableau de nbobj OBJET */
    scene->tabobj = (OBJET *)malloc(scene->nbobj * sizeof(OBJET));
    for (i = 0 ; i < scene->nbobj ; i++)    /* boucle sur les nbobj objets */
    {
        SNPRINTF(signe,255,"Objet %d (nombre de faces)",i);
        if(read_int(file,&line,signe,1,&(scene->tabobj[i].nbface)) )
            return -1;
        /* on reserve de la place pour un tableau
            scene->tabobj[i].nbface de INDICE_FACE */
        scene->tabobj[i].tabface =
            (INDICE_FACE *)malloc(scene->tabobj[i].nbface * sizeof(INDICE_FACE));
        SNPRINTF(signe,255,"Objet %d (liste de faces)",i);
        if(read_int(file,&line,signe,scene->tabobj[i].nbface,scene->tabobj[i].tabface) )
            return -1;
        SNPRINTF(signe,255,"Objet %d (matrice de transformation)",i);
        if(read_transfo4x4d(file,&line,signe,scene->tabobj[i].transfo) )
            return -1;

    }

    /*-----------------------------------------------------------------------*/

    /* on reserve de la place pour un tableau de nbsource SOURCE */
    scene->tabsource = (SOURCE *)malloc(scene->nbsource * sizeof(SOURCE));

    for (i = 0 ; i < scene->nbsource ; i++)    /* boucle sur nbsource sources */
    {

        SNPRINTF(signe,255,"Source %d (ambiante)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabsource[i].ambiante)))
            return -1;
        SNPRINTF(signe,255,"Source %d (couleur)",i);
        if(read_rgb3d(file,&line,signe,&(scene->tabsource[i].couleur)))
            return -1;
        SNPRINTF(signe,255,"Source %d (position)",i);
        if(read_coord4d(file,&line,signe,&(scene->tabsource[i].position)))
            return -1;
        SNPRINTF(signe,255,"Source %d (allure faisceau)",i);
        if(read_allure2d(file,&line,signe,&(scene->tabsource[i].allure_faisceau)))
            return -1;
        SNPRINTF(signe,255,"Source %d (direction)",i);
        if(read_coord3d(file,&line,signe,&(scene->tabsource[i].direction)))
            return -1;
    }

    /* ------------------------------------------------------------------------- */

    if (fclose(file) == EOF)       /* si il y a un probleme avec la fermeture du fichier */
    {
        fprintf(stderr, "\007impossible de fermer le fichier %s\n", filename);
        return(-1);                   /* ca s'est mal passe */
    } else
        return(0); /* tout s'est bien passe */

}

void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

int read_scene_file_3ds(char *filename) {
    FILE *file;                        /* identificateur du fichier */
    int   i, line = 0;
    char  buffer[100];
    char* s;
    char  filename3ds[100];
    float scale = 0.0f;
    int   scene3dsIdx = 0;

    /*-----------------------------------------------------------------------*/

    printf("Lecture du fichier %s\n", filename);

    file = fopen(filename, "r");
    if(!file) {
        fprintf(stderr, "\007\nProbleme a l'ouverture du fichier %s\n", filename);
        return(-1);
    }

    while(!feof(file)) {
        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        trim(buffer);
        // Fill filename
        strcpy(filename3ds, buffer);
        trim(filename3ds);

        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        // Fill Scale
        g_scenes3DS[scene3dsIdx].scale = atof(buffer);

        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        // Fill Translate
        s = strtok(buffer, " ");
        for(i = 0; s != NULL; ++i) {
            g_scenes3DS[scene3dsIdx].translate[i] = atof(s);
            s = strtok(NULL, " ");
        }

        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        // Fill Rotate
        s = strtok(buffer, " ");
        for(i = 0; s != NULL; ++i) {
            g_scenes3DS[scene3dsIdx].rotate[i] = atof(s);
            s = strtok(NULL, " ");
        }

        // Debug printf
        printf("FILENAME : %s - SCALE : %4.2f - TRANSLATE %4.2f %4.2f %4.2f - ROTATE %4.2f %4.2f %4.2f\n"
            , filename3ds, g_scenes3DS[scene3dsIdx].scale
            , g_scenes3DS[scene3dsIdx].translate[0], g_scenes3DS[scene3dsIdx].translate[1], g_scenes3DS[scene3dsIdx].translate[2]
            , g_scenes3DS[scene3dsIdx].rotate[0], g_scenes3DS[scene3dsIdx].rotate[1], g_scenes3DS[scene3dsIdx].rotate[2]);

        // Load 3DS scene
        if(scene3dsIdx >= NB_MAX_3DS_SCENES) {
            fprintf(stderr, "\007\nTrop de fichiers de scene 3DS.\n");
            return(-1);
        }

        if(charge_scene3ds(filename3ds, &g_scenes3DS[scene3dsIdx].lib3dsfile) != -1) {
            ++scene3dsIdx;
        }
    }

    g_nbScenes3DS = scene3dsIdx;

    return 0;
}

void reload_random_objects(char* filename) {
    // First clean previous objects
    memset(g_scenes3DS + (g_nbScenes3DS - g_nbRandomObjects), 0, g_nbRandomObjects * sizeof(SCENE_3DS));
    
    // Reinit variables
    g_nbScenes3DS -= g_nbRandomObjects;
    g_nbRandomObjects = 0;
    // Load random objects
    load_3ds_models_and_randomize(filename);
    load3dsTextures();
}

/*
 *
 */
int load_3ds_models_and_randomize(char* filename) {
    FILE *file;                        /* identificateur du fichier */
    int   i;
    char  buffer[100];
    char  filename3ds[100];
    float scale         = 0.0f;
    int   modele3DSIdx  = 0;
    int   nbModeles3DS  = 0;
    int   objectIndex   = g_nbScenes3DS;
    int   nbObjects;
    Lib3dsFile* tmp3DSLibFiles[NB_MAX_3DS_MODELS];
    float       tmp3DSScale[NB_MAX_3DS_MODELS];
    

    printf("---------------------------------------------\n");
    printf("--------  Random Objects Generation  -------- \n");
    printf("---------------------------------------------\n");
    
    /*-----------------------------------------------------------------------*/
    // First read the file and store a local save of models.

    printf("Lecture du fichier %s\n", filename);

    file = fopen(filename, "r");
    if(!file) {
        fprintf(stderr, "\007\nProbleme a l'ouverture du fichier %s\n", filename);
        return -1;
    }

    while(!feof(file)) {
        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        trim(buffer);
        // Fill filename
        strcpy(filename3ds, buffer);
        trim(filename3ds);

        // Load new line
        fgets(buffer, 100, file);
        while(isspace(buffer[0]) || buffer[0] == '#') {
            fgets(buffer, 100, file);
        }
        // Fill Scale
        tmp3DSScale[nbModeles3DS] = atof(buffer);
        printf("SCALE = %4.2f\n", atof(buffer));

        // Load 3DS scene
        if(nbModeles3DS >= NB_MAX_3DS_MODELS) {
            fprintf(stderr, "\007\nTrop de fichiers de scene 3DS.\n");
            return -1;
        }

        if(charge_scene3ds(filename3ds, &tmp3DSLibFiles[nbModeles3DS]) != -1) {
            ++nbModeles3DS;
        }
    }

    /*-----------------------------------------------------------------------*/
    // Now create a random number of objects using the models.
    printf("---------------------------------------------\n");

    // Number of random objects
    /* initialize random seed: */
    srand(time(NULL));
    nbObjects = rand() % (NB_MAX_3DS_MODELS + 1 - NB_MIN_RANDOM_OBJECTS) + NB_MIN_RANDOM_OBJECTS;
    printf("Number of generated objects : %d\n", nbObjects);

    // Generate each object
    for(i = 0; i < nbObjects; ++i) {
        // Random Model
        modele3DSIdx = rand() % nbModeles3DS;
        // Copy the information of the model.
        g_scenes3DS[objectIndex].lib3dsfile = tmp3DSLibFiles[modele3DSIdx];
        g_scenes3DS[objectIndex].scale      = tmp3DSScale[modele3DSIdx];
        // Random translation
        g_scenes3DS[objectIndex].translate[0]   = rand() % (SCENE_X_MAX + 1 - SCENE_X_MIN) + SCENE_X_MIN;
        g_scenes3DS[objectIndex].translate[1]   = rand() % (SCENE_Y_MAX + 1 - SCENE_Y_MIN) + SCENE_Y_MIN;
        g_scenes3DS[objectIndex].translate[2]   = 0.0f;
        // No rotation
        g_scenes3DS[objectIndex].rotate[0]      = 0.0f;
        g_scenes3DS[objectIndex].rotate[1]      = 0.0f;
        g_scenes3DS[objectIndex].rotate[2]      = 0.0f;
        
        printf("\n");
        printf("Generated object [%d]: \n", objectIndex - g_nbScenes3DS);
        printf("  Model    : %d\n", modele3DSIdx);
        printf("  Scale    : %4.2f\n", g_scenes3DS[objectIndex].scale);
        printf("  Translate: %4.2f %4.2f %4.2f\n", g_scenes3DS[objectIndex].translate[0], g_scenes3DS[objectIndex].translate[1], g_scenes3DS[objectIndex].translate[2]);
        printf("  Rotate   : %4.2f %4.2f %4.2f\n", g_scenes3DS[objectIndex].rotate[0], g_scenes3DS[objectIndex].rotate[1], g_scenes3DS[objectIndex].rotate[2]);

        ++objectIndex;
    }
    printf("---------------------------------------------\n");

    // Add Random 3DS objects to the number of scenes.
    g_nbScenes3DS += nbObjects;
    g_nbRandomObjects = nbObjects;

    return 0;
}

/*-----------------------------------------------------------------------*/
/* affichage des donnees */
/*-----------------------------------------------------------------------*/

void print_scene_data(SCENE *scene)
{
    int i,j;

    printf("---------------------------------------------\n");
    printf("scene lue :\n");
    printf("nombre de fichiers texture :\t%d\n",   scene->nbtex);
    printf("nombre de coordonées de texture:\t%d\n",   scene->nbtc);
    printf("nombre de points :\t%d\n",   scene->nbpt);
    printf("nombre de matieres :\t%d\n", scene->nbmat);
    printf("nombre de faces :\t%d\n",    scene->nbface);
    printf("nombre d'objets :\t%d\n",    scene->nbobj);
    printf("nombre de sources :\t%d\n",  scene->nbsource);
    printf("---------------------------------------------\n");

    for (i = 0 ; i < scene->nbtex ; i++)  /* boucle sur les fichier de texture */
    {
        printf("fichier de texture no %d :\t%s\n",
                i,
                scene->tabtex[i].filename);
    }
    printf("---------------------------------------------\n");


    for (i = 0 ; i < scene->nbtc ; i++)  /* boucle sur les nombre de tc */
    {
        printf("coordonées de texture no %d :\t%lf\t%lf\n",
                i,
                scene->tabtc[i].x,
                scene->tabtc[i].y);
    }
    printf("---------------------------------------------\n");

    for (i = 0 ; i < scene->nbpt ; i++)  /* boucle sur les nombre de pt */
    {
        printf("point no %d :\t%lf\t%lf\t%lf\t%lf\n",
                i,
                scene->tabpt[i].x,
                scene->tabpt[i].y,
                scene->tabpt[i].z,
                scene->tabpt[i].w);
    }
    printf("---------------------------------------------\n");

    for (i = 0 ; i < scene->nbmat ; i++) {
        printf("matiere no %d :\n", i);
        printf("\talpha \t\t= %lf\n",scene->tabmat[i].alpha);
        printf("\tambiante \t= %lf\t%lf\t%lf\n",
                scene->tabmat[i].ambiante.r,
                scene->tabmat[i].ambiante.g,
                scene->tabmat[i].ambiante.b);
        printf("\tdiffuse \t= %lf\t%lf\t%lf\n",
                scene->tabmat[i].diffuse.r,
                scene->tabmat[i].diffuse.g,
                scene->tabmat[i].diffuse.b);
        printf("\temission \t= %lf\t%lf\t%lf\n",
                scene->tabmat[i].emission.r,
                scene->tabmat[i].emission.g,
                scene->tabmat[i].emission.b);

        printf("\tspeculaire \t= %lf\t%lf\t%lf\n",
                scene->tabmat[i].speculaire.r,
                scene->tabmat[i].speculaire.g,
                scene->tabmat[i].speculaire.b);

        printf("\tbrillance \t= %lf\n", scene->tabmat[i].brillance);
        printf("\ttexture \t= %d\n", scene->tabmat[i].tex);
    }
    printf("---------------------------------------------\n");


    for (i = 0 ; i < scene->nbface ; i++)      /* boucle sur les nbface faces */
    {
        printf("face %d : %d points\n",i , scene->tabface[i].nbpt);
        printf("\t\tindices des points : ");
        for (j = 0 ; j < scene->tabface[i].nbpt ; j++) /* boucle sur les nbpt points de la face */
                printf("\t%d",scene->tabface[i].tabpt[j]);
        printf("\n\tindice matiere : \t%d\n", scene->tabface[i].imat);

        printf("\t %d coordonées de texture\n" , scene->tabface[i].nbtc);
        printf("\t\tindices des coordonées de texture: ");
        for (j = 0 ; j < scene->tabface[i].nbtc ; j++) /* boucle sur les nbtc coordonées de texture */
            printf("\t%d",scene->tabface[i].tabtc[j]);
        printf("\n");
    }
    printf("---------------------------------------------\n");


    for (i = 0 ; i < scene->nbobj ; i++) {
        printf("objet %d : %d faces\n",i, scene->tabobj[i].nbface);
        for (j = 0 ; j < scene->tabobj[i].nbface ; j++) {
                printf("\tindices des faces : ");
                printf( "\t%d\n",scene->tabobj[i].tabface[j]);
        }

        printf("\tMatrice:\n\t%lf\t%lf\t%lf\t%lf\n",
                scene->tabobj[i].transfo[0],
                scene->tabobj[i].transfo[4],
                scene->tabobj[i].transfo[8],
                scene->tabobj[i].transfo[12]);
        printf("\t%lf\t%lf\t%lf\t%lf\n",
                scene->tabobj[i].transfo[1],
                scene->tabobj[i].transfo[5],
                scene->tabobj[i].transfo[9],
                scene->tabobj[i].transfo[13]);
        printf("\t%lf\t%lf\t%lf\t%lf\n",
                scene->tabobj[i].transfo[2],
                scene->tabobj[i].transfo[6],
                scene->tabobj[i].transfo[10],
                scene->tabobj[i].transfo[14]);
        printf("\t%lf\t%lf\t%lf\t%lf\n",
                scene->tabobj[i].transfo[3],
                scene->tabobj[i].transfo[7],
                scene->tabobj[i].transfo[11],
                scene->tabobj[i].transfo[15]);


    }
    printf("---------------------------------------------\n");


    for (i = 0 ; i < scene->nbsource ; i++) {
        printf("source no %d :\n", i);
        printf("\tambiante \t: %lf\t%lf\t%lf\n",
                scene->tabsource[i].ambiante.r,
                scene->tabsource[i].ambiante.g,
                scene->tabsource[i].ambiante.b);

        printf("\tcouleur \t: %lf\t%lf\t%lf\n",
                scene->tabsource[i].couleur.r,
                scene->tabsource[i].couleur.g,
                scene->tabsource[i].couleur.b);

        printf("\tposition \t: %lf\t%lf\t%lf\t%lf\n",
                scene->tabsource[i].position.x,
                scene->tabsource[i].position.y,
                scene->tabsource[i].position.z,
                scene->tabsource[i].position.w);

        printf("\tallure faisc. \t: %lf\t%lf\n",
                scene->tabsource[i].allure_faisceau.k,
                scene->tabsource[i].allure_faisceau.theta);

        printf("\tdirection \t: %lf\t%lf\t%lf\n",
                scene->tabsource[i].direction.x,
                scene->tabsource[i].direction.y,
                scene->tabsource[i].direction.z);
        printf("---------------------------------------------\n");
    }
}

/*-------------------------------------------------------------------------*/
