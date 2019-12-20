#include "MMU.h"

void InitMemory(Memory* mem)
{
    for(int i = 0; i < 4096; i++)
    {
        mem->DATA[i] = 0;
        mem->ROM[i] = 0;
        mem->BSS[i] = 0;
        mem->TEXT[i] = 0xff;
    }
}

unsigned char readByte(Memory* mem, Address addr)
{
    return mem->ROM[addr];
}

unsigned short readWord(Memory* mem, Address addr)
{
    return mem->ROM[addr] << 8 + mem->ROM[addr+1];
}

unsigned int readDWord(Memory* mem, Address addr)
{
    return (unsigned int)(mem->ROM[addr] << 24) + (unsigned int)(mem->ROM[addr+1] << 16) + 
           (unsigned int)(mem->ROM[addr+2] << 8) + mem->ROM[addr+3];
}

unsigned int readCode(Memory* mem, Address addr)
{
    return (unsigned int)(mem->TEXT[addr] << 24) + (unsigned int)(mem->TEXT[addr+1] << 16) + 
           (unsigned int)(mem->TEXT[addr+2] << 8) + (unsigned int)(mem->TEXT[addr+3]);
}

void writeByte(Memory* mem, Address addr, unsigned char val)
{
    mem->ROM[addr] = val;
}

void writeWord(Memory* mem, Address addr, unsigned short val)
{
    writeByte(mem, addr, val >> 8);
    writeByte(mem, addr+1, val & 0xff);
}

void writeDWord(Memory* mem, Address addr, unsigned int val)
{
    writeByte(mem, addr, val >> 24);
    writeByte(mem, addr+1, (val & 0xff0000) >> 16);
    writeByte(mem, addr+2, (val & 0xff00) >> 8);
    writeByte(mem, addr+3, val & 0xff);
}

void writeCode(Memory* mem, Stack* code)
{
    int i = 0;
    Stack* tmp = code;
    while(tmp != NULL)
    {
        unsigned val = (int)tmp->value;
        mem->TEXT[i++] = val >> 24;
        mem->TEXT[i++] = (val & 0xff0000) >> 16;
        mem->TEXT[i++] = (val & 0xff00) >> 8;
        mem->TEXT[i++] = val & 0xff;
        tmp = tmp->next;
    }
}

void print_text_segment(Memory* mem)
{
    unsigned int code = readCode(mem, 0);
    for(int i = 0; code != 0xffffffff; i+=4)
    {
        char* assm = translate_op_asm(code);
        printf("    0x%08x 0x%08x  {%s}\n", i, code, assm);
        free(assm);
        code = readCode(mem, i+4);
    }
}

void print_text_segment_arrow(Memory* mem, unsigned int pc)
{
    unsigned int code = readCode(mem, 0);
    for(int i = 0; code != 0xffffffff; i+=4)
    {
        char* assm = translate_op_asm(code);
        if(i == pc)
            printf("--> ");
        else
            printf("    ");
        printf("0x%08x 0x%08x  {%s}\n", i, code, assm);
        free(assm);
        code = readCode(mem, i+4);
    }
}

void MMU_Print(Memory* mem, size_t length)
{
    for(int i = 0; i < length; i++)
    {
        if(i%16 == 0)
            printf("0x%08x: ", i);
        printf("%02x ", mem->ROM[i]);
        if(i%16 == 15)
            printf("\n");
    }
}