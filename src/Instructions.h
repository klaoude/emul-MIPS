#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Utils.h"

unsigned int translate_asm_op(char* line);
char* translate_op_asm(unsigned int op);

#endif