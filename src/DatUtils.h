#ifndef _DAT_UTILS_H
#define _DAT_UTILS_H

#include <stdio.h>

typedef enum {NONE_VALUE,INT_VALUE,DOUBLE_VALUE,STR_VALUE} VALUE_TYPE;


struct ValueList
{
        VALUE_TYPE type;
        union{
                int   intValue;
                double doubleValue;
                char *strValue;
        };
        struct ValueList* next;
};

typedef struct ValueList ValueList;

ValueList* readNextLine( FILE * file,int *num);

void freeValueList(ValueList *valueList);

int printValueList(ValueList *valueList);
int printValue(ValueList *value);

void dumpDATFile(char *filename);

#endif
