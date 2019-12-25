#include "Utils.h"

void trim(char** str)
{
    for(int i = 0; (*str)[i] != 0; i++)
    {
        if((*str)[i] == 0xa || (*str)[i] == 0xd)
            (*str)[i] = 0;
    }
}

void toUpper(char** str)
{
    for(int i = 0; i < strlen(*str); i++)
        (*str)[i] = toupper((*str)[i]);
}

Stack* split(char* str, const char* delim)
{
    if(str == NULL || *str == 0)
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

char* strnewline(char* old, char* append)
{
    unsigned int tmplen = strlen(append);
    unsigned int oldLen = (old == NULL ? 0 : strlen(old));
    char* newstr = (char*)malloc(oldLen + tmplen + 2);
    strncpy(newstr, old, oldLen);
    strncpy(newstr + oldLen, append, tmplen);
    newstr[oldLen + tmplen] = '\n';
    newstr[oldLen + tmplen + 1] = 0;

    if(old != NULL)
        free(old);
    
    char* ret = strdup(newstr);
    free(newstr);
    return ret;
}

char* remove_comments(char* str)
{
    char* no_comment = NULL;
    Stack* lines = split(str, "\n");
    for(int i = 0; i < Stack_length(lines); i++)
    {
        char* tmp = strdup((char*)Stack_At(lines, i));
        char* tofree = tmp;
        while(*tmp == ' ')
            tmp++;

        if(tmp[0] == '#')
        {
            free(tofree);
            continue;
        }

        remove_after_comment(&tmp);

        no_comment = strnewline(no_comment, tmp);
    }

    Stack_free(&lines);
    free(str);
    return no_comment;
}

void replace_multi_space_with_single_space(char *str)
{
    char *dest = str;

    while (*str)
    {
        while (*str == ' ' && *(str + 1) == ' ')
            str++;

       *dest++ = *str++;
    }
 
    *dest = 0;
}

char* translate_pseudoMIPS(char* str)
{
    char* translated = NULL;
    Stack* lines = split(str, "\n");
    for(int i = 0; i < Stack_length(lines); i++)
    {
        char* line = (char*)Stack_At(lines, i);
        if(strcmp(line, "") == 0)
            continue;
            
        char* tmp = strdup(line);

        Stack* split_stack = nSplit(line, " ", 1);
        char* opName = (char*)Stack_At(split_stack, 0);

        if(strcasecmp(opName, "move") == 0)
        {
            Stack* args = split((char*)Stack_At(split_stack, 1), ",");
            char* arg0 = (char*)Stack_At(args, 0);
            char* arg1 = (char*)Stack_At(args, 1);

            remove_space(&arg0);
            remove_space(&arg1);

            char* newstr = (char*)malloc(13 + strlen(arg0) + strlen(arg1));
            sprintf(newstr, "or %s, %s, $zero", arg0, arg1);
            translated = strnewline(translated, newstr);

            Stack_free(&args);
        }
        else if(strcasecmp(opName, "clear") == 0)
        {
            char* arg = (char*)Stack_At(split_stack, 1);
            char* newstr = (char*)malloc(18 + strlen(arg));
            sprintf(newstr, "or %s, $zero, $zero", arg);
            translated = strnewline(translated, newstr);
        }
        else if(strcasecmp(opName, "la") == 0 || strcasecmp(opName, "li") == 0)
        {
            Stack* args = split((char*)Stack_At(split_stack, 1), ",");
            char* arg0 = (char*)Stack_At(args, 0);
            char* arg1 = (char*)Stack_At(args, 1);

            remove_space(&arg1);

            if(*arg1 >= '0' && *arg1 <= '9')
            {
                unsigned int arg1_num = atoi(arg1);
                if(arg1_num > 0xffff)
                {
                    char* newstr = (char*)malloc(7 + strlen(arg0) + 5);
                    sprintf(newstr, "lui %s, %d", arg0, (arg1_num & 0xffff0000) >> 16);
                    translated = strnewline(translated, newstr);

                    newstr = (char*)malloc(9 + 2 * strlen(arg0) + 5);
                    sprintf(newstr, "ori %s, %s, %d", arg0, arg0, arg1_num & 0xffff);
                    translated = strnewline(translated, newstr);
                }
                else
                {
                    char* newstr = (char*)malloc(15 + strlen(arg0) + 5);
                    sprintf(newstr, "ori %s, $zero, %d", arg0, arg1_num);
                    translated = strnewline(translated, newstr);
                }                
            }
            else
            {
                char* newstr = (char*)malloc(7 + strlen(arg0) + strlen(arg1) + 3);
                sprintf(newstr, "lui %s, %s|HI", arg0, arg1);
                translated = strnewline(translated, newstr);

                newstr = (char*)malloc(9 + 2 * strlen(arg0) + strlen(arg1) + 3);
                sprintf(newstr, "ori %s, %s, %s|LO", arg0, arg0, arg1);
                translated = strnewline(translated, newstr);      
            }     

            Stack_free(&args);
        }
        else if(strcasecmp(opName, "mul") == 0)
        {
            Stack* args = split((char*)Stack_At(split_stack, 1), ",");
            char* arg0 = (char*)Stack_At(args, 0);
            char* arg1 = (char*)Stack_At(args, 1);
            char* arg2 = (char*)Stack_At(args, 2);

            char* newstr = (char*)malloc(8 + strlen(arg1) + strlen(arg2));
            sprintf(newstr, "mult %s, %s", arg1, arg2);
            translated = strnewline(translated, newstr);

            newstr = (char*)malloc(6 + strlen(arg0));
            sprintf(newstr, "mflo %s", arg0);
            translated = strnewline(translated, newstr);

            Stack_free(&args);
        }
        else
            translated = strnewline(translated, line);

        Stack_free(&split_stack);

        free(tmp);
    }
    Stack_free(&lines);

    return translated;
}

Stack* getSectionContent(char* str, char* section)
{
    Stack* ret = Stack_Init();

    char status = 0; // 0 = not in section, 1 = in section
    char* line = strdup(str);
    char* toFree = line;
    while(line)
    {
        char* nextline = strchr(line, '\n');
        if(nextline)
            *nextline = 0;

        replace_multi_space_with_single_space(line);

        switch(status)
        {
        case 0:
            remove_space(&line);
            if(!strcmp(line, section))
                status = 1;
            break;
        case 1:
            if(*line == '.' && strcmp(line+1, section))
                status = 0;
            else if(*line != 0)
                Stack_Insert(&ret, strdup(line));
            break;
        }

        if(nextline)
            *nextline = '\n';
        
        line = nextline ? nextline + 1 : NULL;
    }

    free(toFree);

    return ret;
}

InstructionFormat getInstructionFmt(char* op)
{
    if(strcmp(op, "ADD") == 0 || strcmp(op, "SUB") == 0 || strcmp(op, "MULT") == 0 || strcmp(op, "DIV") == 0 || strcmp(op, "AND") == 0 ||
       strcmp(op, "OR") == 0  || strcmp(op, "XOR") == 0 || strcmp(op, "ROTR") == 0 || strcmp(op, "SLL") == 0 || strcmp(op, "SRL") == 0 ||
       strcmp(op, "SLT") == 0 || strcmp(op, "MFHI") == 0 || strcmp(op, "MFLO") == 0 || strcmp(op, "JR") == 0)
        return R;
    else if(strcmp(op, "ADDI") == 0 || strcmp(op, "LW") == 0 || strcmp(op, "SW") == 0 || strcmp(op, "LUI") == 0 || strcmp(op, "BEQ") == 0 ||
            strcmp(op, "BNE") == 0 || strcmp(op, "BGTZ") == 0 || strcmp(op, "BLEZ") == 0 || strcmp(op, "ORI") == 0)
        return I;
    else if(strcmp(op, "J") == 0 || strcmp(op, "JAL") == 0)
        return J;
}

unsigned char to_register_code(char* register_name)
{
    if(!strcmp(register_name+1, "zero"))
        return 0;
    else if(!strcmp(register_name+1, "at"))
        return 1;
    else if(!strcmp(register_name+1, "gp"))
        return 28;
    else if(!strcmp(register_name+1, "sp"))
        return 29;
    else if(!strcmp(register_name+1, "fp"))
        return 30;
    else if(!strcmp(register_name+1, "ra"))
        return 31;
    else if(register_name[1] == 'v')
        return atoi(register_name+2)+2;
    else if(register_name[1] == 'a')
        return atoi(register_name+2)+4;
    else if(register_name[1] == 't')
        return atoi(register_name+2)+8;
    else if(register_name[1] == 's')
        return atoi(register_name+2)+16;
    else if(register_name[1] == 't')
        return atoi(register_name+2)+24;
    else if(register_name[1] == 'k')
        return atoi(register_name+2)+26;
    else
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
    else if(strcmp(op, "ORI") == 0)
        return 0xd;
    return -1;    
}

void flush()
{
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF) ;
}