#include "Utils.h"

void trim(char** str)
{
    for(int i = 0; (*str)[i] != 0; i++)
    {
        if((*str)[i] == 0xa || (*str)[i] == 0xd)
            (*str)[i] = 0;
    }
}

Stack* split(char* str, const char* delim)
{
    if(str == NULL)
        return NULL;

    Stack* stack = Stack_Init();

    char* token = NULL, *tmp = NULL, *toFree = NULL;
    toFree = tmp = strdup(str);
    while(token = strsep(&tmp, delim))
        Stack_Insert(&stack, (void*)strdup(token));

    free(toFree);

    return stack;
}

Stack* nSplit(char* str, const char* delim, unsigned int n)
{
    if(str == NULL)
        return NULL;

    Stack* stack = Stack_Init();

    unsigned int i = 0;
    char* token = NULL, *tmp = NULL, *toFree = NULL;
    toFree = tmp = strdup(str);
    while(i != n && (token = strsep(&tmp, delim)))
    {
        Stack_Insert(&stack, (void*)strdup(token));
        i++;
    }
    if(tmp != NULL)
        Stack_Insert(&stack, (void*)strdup(tmp));  

    free(toFree);

    return stack;
}

void remove_space(char** str)
{
    if(*str == NULL)
        return;
    int count = 0;
    for(int i = 0; (*str)[i]; i++)
        if((*str)[i] != ' ')
            (*str)[count++] = (*str)[i];
    (*str)[count] = 0;
}

void remove_after_comment(char** str)
{
    if(*str == NULL)
        return;

    for(int i = 0; (*str)[i]; i++)
    {
        if((*str)[i] == '#')
        {
            (*str)[i] = 0;
            return;
        }
    }
}

InstructionFormat getInstructionFmt(char* op)
{
    if(strcmp(op, "ADD") == 0 || strcmp(op, "SUB") == 0 || strcmp(op, "MULT") == 0 || strcmp(op, "DIV") == 0 || strcmp(op, "AND") == 0 ||
       strcmp(op, "OR") == 0  || strcmp(op, "XOR") == 0 || strcmp(op, "ROTR") == 0 || strcmp(op, "SLL") == 0 || strcmp(op, "SRL") == 0 ||
       strcmp(op, "SLT") == 0 || strcmp(op, "MFHI") == 0 || strcmp(op, "MFLO") == 0 || strcmp(op, "JR") == 0)
        return R;
    else if(strcmp(op, "ADDI") == 0 || strcmp(op, "LW") == 0 || strcmp(op, "SW") == 0 || strcmp(op, "LUI") == 0 || strcmp(op, "BEQ") == 0 ||
            strcmp(op, "BNE") == 0 || strcmp(op, "BGTZ") == 0 || strcmp(op, "BLEZ") == 0)
        return I;
    else if(strcmp(op, "J") == 0 || strcmp(op, "JAL") == 0)
        return J;
}

unsigned char to_register_code(char* register_name)
{
    return atoi(register_name+1);
}

unsigned char to_operation_code(char* op)
{
    if(strcmp(op, "ADD") == 0)
        return 32;
    else if(strcmp(op, "ADDI") == 0)
        return 8;
    else if(strcmp(op, "AND") == 0)
        return 36;
    else if(strcmp(op, "BEQ") == 0)
        return 4;
    else if(strcmp(op, "BGTZ") == 0)
        return 7;
    else if(strcmp(op, "BLEZ") == 0)
        return 6;
    else if(strcmp(op, "BNE") == 0)
        return 5;
    else if(strcmp(op, "DIV") == 0)
        return 26;
    else if(strcmp(op, "J") == 0)
        return 2;
    else if(strcmp(op, "JAL") == 0)
        return 3;
    else if(strcmp(op, "JR") == 0)
        return 8;
    else if(strcmp(op, "LUI") == 0)
        return 0xf;
    else if(strcmp(op, "LW") == 0)
        return 0x23;
    else if(strcmp(op, "MFHI") == 0)
        return 0x10;
    else if(strcmp(op, "MFLO") == 0)
        return 0x12;
    else if(strcmp(op, "MULT") == 0)
        return 0x18;
    else if(strcmp(op, "NOP") == 0)
        return 0;
    else if(strcmp(op, "OR") == 0)
        return 0x25;
    else if(strcmp(op, "ROTR") == 0)
        return 2;
    else if(strcmp(op, "SLL") == 0)
        return 0;
    else if(strcmp(op, "SLT") == 0)
        return 0x2a;
    else if(strcmp(op, "SRL") == 0)
        return 2;
    else if(strcmp(op, "SUB") == 0)
        return 0x22;
    else if(strcmp(op, "SW") == 0)
        return 0x2b;
    else if(strcmp(op, "SYSCALL") == 0)
        return 12;
    else if(strcmp(op, "XOR") == 0)
        return 0x26;
    return -1;    
}

void flush()
{
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF) ;
}