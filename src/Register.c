#include "Register.h"

#include "MMU.h"

void Registers_Init(Registers* reg)
{
    for(int i = 0; i < 32; i++)
        reg->reg[i] = 0;
    reg->reg[29] = STACK_ADDRESS; //SP
    reg->PC = TEXT_ADDRESS;
    reg->SP = STACK_ADDRESS;
    reg->FP = 0;
    reg->HI = 0;
    reg->LO = 0;
}

void Registers_Print(Registers* reg)
{
    for(int i = 0; i < 32; i+=4)
        printf("$%02d: %d\t\t$%02d: %d\t\t$%02d: %d\t\t$%02d: %d\n", i, reg->reg[i], i+1, reg->reg[i+1], i+2, reg->reg[i+2], i+3, reg->reg[i+3]);

    printf("PC:%d\t\tSP:%d\t\tFP:%d\n", reg->PC, reg->SP, reg->FP);
    printf("HI:%d\t\tLO:%d\n", reg->HI, reg->LO);
}