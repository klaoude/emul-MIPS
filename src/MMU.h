#ifndef MMU_H
#define MMU_H

#include "Stack.h"
#include "Instructions.h"

typedef unsigned int Address;

typedef enum SECTIONS_ADDRESS
{
    DATA_ADDRESS  = 0,
    BSS_ADDRESS   = 0x400,
    TEXT_ADDRESS  = 0x800,
    STACK_ADDRESS = 0x1000
} SECTIONS_ADDRESS;

typedef struct Memory
{
    unsigned char ROM[0xffff];

    Address freeData;
    Address freeBss;
    Address freeText;
    Address freeRom;
} Memory;

typedef enum DATATYPE
{
    ASCIIZ,
    WORD,
    BYTE,
    SPACE,
    HALF,
    FLOAT,
    DOUBLE
} DATATYPE;

void InitMemory(Memory* mem);

unsigned char readByte(Memory* mem, Address addr);
unsigned short readWord(Memory* mem, Address addr);
unsigned int readDWord(Memory* mem, Address addr);
double readDouble(Memory* mem, Address addr);
float readFloat(Memory* mem, Address addr);
char* readString(Memory* mem, Address addr);

unsigned int readCode(Memory* mem, Address addr);

void writeByte(Memory* mem, Address addr, unsigned char val);
void writeWord(Memory* mem, Address addr, unsigned short val);
void writeDWord(Memory* mem, Address addr, unsigned int val);
void writeDouble(Memory* mem, Address addr, double val);
void writeFloat(Memory* mem, Address addr, float val);
void writeString(Memory* mem, Address addr, char* str);

void writeCode(Memory* mem, Stack* code);

void print_text_segment(Memory* mem);
void print_text_segment_arrow(Memory* mem, unsigned int pc);
void MMU_Print(Memory* mem, unsigned int start, size_t length);
Address MMU_alloc(Memory* mem, DATATYPE type, char* data);

#endif