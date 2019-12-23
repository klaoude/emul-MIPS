#ifndef LABEL_H
#define LABEL_H

#include "Utils.h"

// Fix circular inclusion don't forget to include "CPU.h" in Label.c
typedef struct CPU CPU;
typedef unsigned int Address;
typedef enum DATATYPE DATATYPE;
// -----

typedef enum SECTIONS
{
    DATA,
    TEXT,
    BSS
} SECTIONS;

typedef struct Label
{
    Address addr;
    char* name;
    SECTIONS section;
} Label;

void Labels_free(Stack** labels);
DATATYPE getDataType(char* str);
void add_label_from_section(Stack** labels, Stack* section, SECTIONS s, CPU* cpu);

#endif