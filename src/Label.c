#include "Label.h"

#include "CPU.h"

DATATYPE getDataType(char* str)
{
    toUpper(&str);
    if(!strcmp(str, "ASCIIZ"))
        return ASCIIZ;
    else if(!strcmp(str, "WORD"))
        return WORD;
    else if(!strcmp(str, "BYTE"))
        return BYTE;
    else if(!strcmp(str, "SPACE"))
        return SPACE;
    else if(!strcmp(str, "HALF"))
        return HALF;
    else if(!strcmp(str, "FLOAT"))
        return FLOAT;
    else
        return DOUBLE;
}

void add_label_from_section(Stack** labels, Stack* section, SECTIONS s, CPU* cpu)
{
    Stack* tmp = section;
    char inSection = 0;
    Label* l = NULL;
    while(tmp != NULL)
    {
        Stack* match;
        switch(s)
        {
        case DATA:
            match = regex_match((char*)tmp->value, "([A-Za-z0-9_]*): \\.([A-Za-z0-9]*) (.*)");
            l = (Label*)malloc(sizeof(Label));
            l->name = (char*)Stack_At(match, 0);
            l->section = DATA;
            DATATYPE type = getDataType((char*)Stack_At(match, 1));
            l->addr = MMU_alloc(&cpu->memory, type, (char*)Stack_At(match, 2));
            Stack_Insert(labels, l);
            break;
        case TEXT:
            ;
            char* val = (char*)tmp->value;
            char* tmpp;
            if((tmpp = strchr(val, ':')) != NULL)
            {
                if(l != NULL)
                    Stack_Insert(labels, l);
                l = (Label*)malloc(sizeof(Label));
                char* dup = strdup(val);
                dup[strlen(val)-1] = 0;
                l->name = dup;
                l->section = TEXT;
                l->addr = cpu->memory.freeText;
            }
            else
            {
                cpu->memory.freeText += 4;
            }
            break;
        }
        tmp = tmp->next;
    }
}