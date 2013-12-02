/*
 
        Universite Technologique de Compiegne
        
        UV: LO12
        
        FICHIER: DatUtils.c
 
        COMMENTAIRE:
                Lecture du fichier Scene (Bas Niveau)
 
        AUTEURS:
                Veronique BERGE-CHERFAOUI
                DG
                Olivier BEZET  A2002-A2005
                Romain HERAULT A2005-A2006
*/


#include "DatUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Lit une ligne du fichier texte*/
char * readRawLine( FILE * file, int *num)
{
        int    size = 256;
        int    offset = 0;
        char * nu;
        char * str = (char*)malloc( size );

        while( str && fgets(str+ offset, ((int)str) - ((int)offset), file ) ) {
                offset = size - 1;
                if( strrchr( str, '\n' ) == 0 ) {
                        nu = (char *)realloc( str, size += 256 );
                        if( nu == NULL )
                                free( str);
                        str = nu;
                } else
                        break;
        }
        if( offset == 0 && str ) {
                free( str );
                return NULL;
        }
        if(num)
                (*num)++;
        return str;
}

/*Anleve les commentaires et les espaces inutiles d'une ligne du fichier texte*/
char *cleanRawLine(char *line)
{
        char *str=(char*)malloc(strlen(line)+1);
        int i,j;
        int begin=1,space=0;
        for(i=0,j=0;line[i];i++) {
                if(isspace(line[i])) {
                        if(begin)
                                continue;
                        if(space)
                                continue;
                        str[j++]=' ';
                        space=1;
                } else {
                        if(line[i]=='#')
                                break;
                        str[j++]=line[i];
                        begin=0;
                        space=0;
                }
        }

        do {
                if(--j==-1)
                        break;
        } while(isspace(str[j]));

        str[j+1]=0;
        str=(char *)realloc(str,strlen(str)+1);
        return str;
}

/*Lit une valeur d'une ligne*/
int readOneValue(char *line,ValueList *value)
{
        char* pos=line;
        int lui,luf,i;
        double f;

        value->next=NULL;

        while(strlen(pos)) {
                if(!isspace(*pos))
                        break;
                pos++;
        }
        if(strlen(pos)==0) {
                value->type=NONE_VALUE;
                return 0;
        }

        if(sscanf(pos,"%lf%n",&f,&luf)<=0)
                goto str;
        if(sscanf(pos,"%d%n",&i,&lui)<=0)
                goto str;

        if((lui>0)&&(lui==luf)) {
                value->intValue=i;
                value->type=INT_VALUE;
                return pos-line+lui;
        }

        if(luf>0) {
                value->doubleValue=f;
                value->type=DOUBLE_VALUE;
                return pos-line+luf;
        }

str:
        value->strValue=(char*) malloc(256);
        if(sscanf(pos,"%255s%n",value->strValue,&lui)>0) {
                value->type=STR_VALUE;
                return pos-line+lui;
        }

        free(value->strValue);
        value->type=NONE_VALUE;
        return 0;
}

/*Lit toutes les valeurs d'une ligne et les stockes dans une ValueList*/
ValueList *readValueList(char *line)
{
        int lu;
        ValueList *ret=(ValueList *) malloc(sizeof(ValueList));
        if((lu=readOneValue(line,ret))==0) {
                free(ret);
                return NULL;
        } else {
                ret->next=readValueList(line+lu);
                return ret;
        }
}

/*Desalloue une ValueList*/
void freeValueList(ValueList *valueList)
{
        if(valueList==NULL)
                return;
        if(valueList->next)
                freeValueList(valueList->next);
        if(valueList->type==STR_VALUE)
                free(valueList->strValue);
        free(valueList);
}

/*Affiche une valeur*/
int printValue(ValueList *value)
{
        if(value==NULL)
                return -1;
        if(value->type==NONE_VALUE)
                printf("NONE ");
        if(value->type==INT_VALUE)
                printf("INT: %d ",value->intValue);
        if(value->type==DOUBLE_VALUE)
                printf("DOUBLE: %lf ",value->doubleValue);
        if(value->type==STR_VALUE)
                printf("STR: %s ",value->strValue);
        return 0;
}

/*Affiche une toute une ValueList*/
int printValueList(ValueList *valueList)
{
        if(valueList==NULL)
                return -1;
        printValue(valueList);
        if(valueList->next)
                printValueList(valueList->next);
        else
                printf("\n");
        return 0;
}

/*Lit la ligne suivante du fichier et la retourne en ValueList*/
ValueList* readNextLine( FILE * file,int *num)
{
        char *rline=NULL,*cline=NULL;
        do {
                if(rline)
                        free(rline);
                if(cline)
                        free(cline);
                if(feof(file))
                        return NULL;
                if(strlen(rline=readRawLine(file,num))==0)
                        return NULL;
        } while(strlen(cline=cleanRawLine(rline))==0);
        return readValueList(cline);
}

/*Affiche l'ensemble du contenu d'un fichier*/
void dumpDATFile(char *filename)
{
        FILE *file;
        ValueList* vl;
        int num=-1;
        file=fopen(filename,"r");

        while((vl=readNextLine(file,&num))) {
                printf("%d: ",num);
                printValueList(vl);
                freeValueList(vl);
        }
        if(vl)
                freeValueList(vl);
        fclose(file);
}

#ifdef DAT_UTILS_MAIN

int main(int argc,char** argv)
{
        dumpDATFile(argv[1]);
}
#endif
