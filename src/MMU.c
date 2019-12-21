#include "MMU.h"

void InitMemory(Memory* mem)
{
    memset(mem->ROM, 0, 0xffff);
    mem->freeBss = BSS_ADDRESS;
    mem->freeData = DATA_ADDRESS;
    mem->freeRom = STACK_ADDRESS;
    mem->freeText = TEXT_ADDRESS;
    memset(mem->ROM + TEXT_ADDRESS, 0xff, 0x200);
}

Address MMU_alloc(Memory* mem, DATATYPE type, char* data)
{
    Address ret = mem->freeData;
    switch(type)
    {
    case ASCIIZ:
        for(int i = 1; i < strlen(data) - 1; i++)
        {
            writeByte(mem, mem->freeData, data[i]);
            mem->freeData++;
        }
        writeByte(mem, mem->freeData, 0);
        mem->freeData++;
        break;
    case WORD:
        if(!strncmp(data, "0x", 2))
            writeDWord(mem, mem->freeData, (unsigned int)strtol(data, NULL, 16));
        else
            writeDWord(mem, mem->freeData, (unsigned int)strtol(data, NULL, 10));
        mem->freeData += 4;
        break;
    case BYTE:
        if(!strncmp(data, "0x", 2))
            writeByte(mem, mem->freeData, (unsigned char)strtol(data, NULL, 16));
        else
            writeByte(mem, mem->freeData, (unsigned char)strtol(data, NULL, 10));
        mem->freeData++;
        break;
    case SPACE:
        if(!strncmp(data, "0x", 2))
            mem->freeData += strtol(data, NULL, 16);
        else
            mem->freeData += strtol(data, NULL, 10);
        break;
    case HALF:
        if(!strncmp(data, "0x", 2))
            writeByte(mem, mem->freeData, (unsigned short)strtol(data, NULL, 16));
        else
            writeByte(mem, mem->freeData, (unsigned short)strtol(data, NULL, 10));
        mem->freeData += 2;
        break;
    case FLOAT:
        // TODO
        mem->freeData += 4;
        break;
    case DOUBLE:
        //TODO
        mem->freeData += 8;
        break;
    }

    return ret;
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
    return (unsigned int)(mem->ROM[addr] << 24) + (unsigned int)(mem->ROM[addr+1] << 16) + 
           (unsigned int)(mem->ROM[addr+2] << 8) + (unsigned int)(mem->ROM[addr+3]);
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
    int i = TEXT_ADDRESS;
    Stack* tmp = code;
    while(tmp != NULL)
    {
        unsigned val = (int)tmp->value;
        writeDWord(mem, i, val);
        i += 4;
        tmp = tmp->next;
    }
}

void print_text_segment(Memory* mem)
{
    unsigned int code = readCode(mem, TEXT_ADDRESS);
    for(int i = TEXT_ADDRESS; code != 0xffffffff; i+=4)
    {
        char* assm = translate_op_asm(code);
        printf("    0x%08x 0x%08x  {%s}\n", i, code, assm);
        free(assm);
        code = readCode(mem, i+4);
    }
}

void print_text_segment_arrow(Memory* mem, unsigned int pc)
{
    unsigned int code = readCode(mem, TEXT_ADDRESS);
    for(int i = TEXT_ADDRESS; code != 0xffffffff; i+=4)
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

void MMU_Print(Memory* mem, unsigned int start, size_t length)
{
    for(unsigned int i = start; i < start + length; i++)
    {
        if(i%16 == 0)
            printf("0x%08x: ", i);
        printf("%02x ", mem->ROM[i]);
        if(i%16 == 15)
            printf("\n");
    }
}