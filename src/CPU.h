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

//Init the CPU
void CPU_Init(CPU* cpu);
//Main loop of the CPU
void CPU_Main(CPU* cpu, char pas, unsigned char silent);
void CPU_Execute(CPU* cpu);

#endif