#ifndef CPU_H
#define CPU_H

#include "Register.h"
#include "MMU.h"

typedef struct CPU
{
    Registers registers;
    Memory memory;
    unsigned char running;
} CPU;

void CPU_Init(CPU* cpu);
void CPU_Main(CPU* cpu, char pas);
void CPU_Execute(CPU* cpu);

#endif