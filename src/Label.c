#include "Label.h"

#include "CPU.h"

void Labels_free(Stack** labels)
{
    Stack* tmp = *labels;
    while(tmp != NULL)
    {
        Stack* next = tmp->next;
        if(tmp->value != NULL)
        {
            if(((Label*)tmp->value)->name != NULL)
                free(((Label*)tmp->value)->name);
            free(tmp->value);
            tmp->value = NULL;
        }
        free(tmp);
        tmp = next;
    }
}

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
            l->name = strdup((char*)Stack_At(match, 0));
            l->section = DATA;
            DATATYPE type = getDataType((char*)Stack_At(match, 1));
            l->addr = MMU_alloc(&cpu->memory, type, (char*)Stack_At(match, 2));
            Stack_Insert(labels, l);
            Stack_free(&match);
            break;
        case TEXT:
            ;
            char* val = (char*)tmp->value;
            if(!val)
            {
                tmp = tmp->next;
                continue;
            }

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

    if(l != NULL && s == TEXT)
        Stack_Insert(labels, l);
}