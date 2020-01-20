#ifndef REGISTER_H
#define REGISTER_H

#include "stdio.h"

typedef unsigned long Register;

typedef struct Registers
{
    Register reg[32];
    Register HI, LO, PC, SP, FP;
} Registers;

void Registers_Init(Registers* reg);
void Registers_Print(Registers* reg);

#endif