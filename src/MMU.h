#ifndef MMU_H
#define MMU_H

#include "Stack.h"
#include "Instructions.h"

typedef struct Memory
{
    unsigned char DATA[4096]; //for .data segment
    unsigned char BSS[4096]; //for .bss segment
    unsigned char TEXT[4096]; //for .text segment
    unsigned char ROM[4096]; //for memory
} Memory;

typedef unsigned int Address;

void InitMemory(Memory* mem);

unsigned char readByte(Memory* mem, Address addr);
unsigned short readWord(Memory* mem, Address addr);
unsigned int readDWord(Memory* mem, Address addr);

unsigned int readCode(Memory* mem, Address addr);

void writeByte(Memory* mem, Address addr, unsigned char val);
void writeWord(Memory* mem, Address addr, unsigned short val);
void writeDWord(Memory* mem, Address addr, unsigned int val);

void writeCode(Memory* mem, Stack* code);

void print_text_segment(Memory* mem);
void print_text_segment_arrow(Memory* mem, unsigned int pc);
void MMU_Print(Memory* mem, size_t length);

#endif