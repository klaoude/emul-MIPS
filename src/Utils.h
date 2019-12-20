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

void trim(char** str);
void toUpper(char** str);
Stack* getSectionContent(char* str, char* section);
void remove_start_space(char** str);
void remove_space(char** str);
void remove_after_comment(char** str);
char* remove_comments(char* str);
Stack* split(char* str, const char* delim);
Stack* nSplit(char* str, const char* delim, unsigned int n);
Stack* get_offset_base(char* str);

InstructionFormat getInstructionFmt(char* op);
unsigned char to_register_code(char* register_name);
unsigned char to_operation_code(char* op);

void flush();

#endif