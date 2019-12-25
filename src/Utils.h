#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>
#include <string.h>
#include <ctype.h>

#include "Stack.h"
#include "Regex.h"

typedef enum InstructionFormat
{
    R,
    I,
    J,
    NOP,
    UNKNOW
} InstructionFormat;

//Remove \n and \r in string
void trim(char** str);

//set all alpha value in the string to the upper one
void toUpper(char** str);

Stack* getSectionContent(char* str, char* section);

//Remove the spaces at the start of the string
void remove_start_space(char** str);
//remove all space in string
void remove_space(char** str);
//remove everything after # in string
void remove_after_comment(char** str);
//remove all comment in string
char* remove_comments(char* str);
//translate the pseudoMIPS to MIPS
char* translate_pseudoMIPS(char* str);
//Return a Stack with string splited
Stack* split(char* str, const char* delim);
//Return a stack with string splitted but stop after n split
Stack* nSplit(char* str, const char* delim, unsigned int n);

//return the number of register
unsigned char to_register_code(char* register_name);
//return the OPCode
unsigned char to_operation_code(char* op);

//flush stdin
void flush();

#endif