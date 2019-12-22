#include "Instructions.h"

int write_typeR_instr(unsigned char opcode, unsigned char arg1, unsigned char arg2, unsigned char arg3, unsigned char special)
{
    int hex = 0;
    hex &= 0x1fffffff; //Special
    hex |= arg1 << 21; // rs
    hex |= arg2 << 16; // rt
    hex |= arg3 << 11; // rd
    hex += (special << 6) & 0x7c0; // sa
    hex += opcode; //function
    return hex;
}

int write_typeI_instr(unsigned char opcode, unsigned char arg1, unsigned char arg2, unsigned short immediate)
{
    int hex = opcode << 26; //opcode
    hex |= arg1 << 21; //rs
    hex |= arg2 << 16; //rt
    hex += immediate; //immediatearg1
    return hex;
}

int write_typeJ_instr(unsigned char op, int index)
{
    int hex = op << 26; //opcode
    hex += index & 0x3ffffff;//instr_index
    return hex;
}

// use of unused bits of regvalue
// 000x xxxx -> register => $ infront
// 001x xxxx -> direct value
// 1000 0000 -> unused
// 01yx xxxx -> last
char* read_typeR_instr(const char* opname, char arg1, char arg2, char arg3)
{
    char* ret = (char*)calloc(20, 1);
    strcpy(ret, opname);
    char* tmp = ret + strlen(ret);

    char* reg = (char*)calloc(6, 1);

    if((arg1&0x80) == 0)
    {
        if((arg1 & 0x20) == 0x20)
        {
            if((arg1 & 0x40) == 0x40)
                sprintf(reg, "%d", arg1&0x9f);
            else
                sprintf(reg, "%d, ", arg1&0xdf);
        }
        else
        {
            if((arg1 & 0x40) == 0x40)
                sprintf(reg, "$%d", arg1&0xbf);
            else
                sprintf(reg, "$%d, ", arg1);
        }
        strcpy(tmp, reg);
        tmp += strlen(reg);
    }    

    if((arg2&0x80) == 0)
    {
        if((arg2 & 0x20) == 0x20)
        {
            if((arg2 & 0x40) == 0x40)
                sprintf(reg, "%d", arg2&0x9f);
            else
                sprintf(reg, "%d, ", arg2&0xdf);
        }
        else
        {
            if((arg2 & 0x40) == 0x40)
                sprintf(reg, "$%d", arg2&0xbf);
            else
                sprintf(reg, "$%d, ", arg2);
        }
        strcpy(tmp, reg);
        tmp += strlen(reg);
    }

    if((arg3&0x80) == 0)
    {
        if((arg3 & 0x20) == 0x20)
        {
            if((arg3 & 0x40) == 0x40)
                sprintf(reg, "%d", arg3&0x9f);
            else
                sprintf(reg, "%d, ", arg3&0xdf);
        }
        else
        {
            if((arg3 & 0x40) == 0x40)
                sprintf(reg, "$%d", arg3&0xbf);
            else
                sprintf(reg, "$%d, ", arg3);
        }
        strcpy(tmp, reg);
        tmp += strlen(reg);
    }    

    *tmp = 0;

    free(reg);

    return ret;
}

// use of unused bits of regvalue
// 000x xxxx -> register => $ infront
// 001x xxxx -> direct value
// 1000 0000 -> unused
// 01yx xxxx -> last
char* read_typeI_instr(const char* opname, char arg1, char arg2, short offset)
{
    char* ret = (char*)calloc(25, 1);
    strcpy(ret, opname);
    char* tmp = ret + strlen(ret);

    char* reg = (char*)calloc(6, 1);

    if((arg1&0x80) == 0)
    {
        if((arg1 & 0x20) == 0x20)
        {
            if((arg1 & 0x40) == 0x40)
                sprintf(reg, "%d", arg1&0x9f);
            else
                sprintf(reg, "%d, ", arg1&0xdf);
        }
        else
        {
            if((arg1 & 0x40) == 0x40)
                sprintf(reg, "$%d", arg1&0xbf);
            else
                sprintf(reg, "$%d, ", arg1);
        }
        strcpy(tmp, reg);
        tmp += strlen(reg);
    }    

    if((arg2&0x80) == 0)
    {
        if((arg2 & 0x20) == 0x20)
        {
            if((arg2 & 0x40) == 0x40)
                sprintf(reg, "%d", arg2&0x9f);
            else
                sprintf(reg, "%d, ", arg2&0xdf);
        }
        else
        {
            if((arg2 & 0x40) == 0x40)
                sprintf(reg, "$%d", arg2&0xbf);
            else
                sprintf(reg, "$%d, ", arg2);
        }
        strcpy(tmp, reg);
        tmp += strlen(reg);
    }

    sprintf(reg, "%d", offset);
    strcpy(tmp, reg);
    tmp += strlen(reg);

    *tmp = 0;

    free(reg);

    return ret;
}

char* read_typeJ_instr(const char* opname, int index)
{
    char* ret = (char*)malloc(15);
    strcpy(ret, opname);
    sprintf(ret + strlen(ret), "%d", index);
    return ret;
}

unsigned int translate_asm_op(char* line)
{
    if(line[0] == '#')
        return -1;

    while(*line == ' ')
        line++;

    remove_after_comment(&line);

    Stack* opCodes = nSplit(line, " ", 1);
    char* op = (char*)Stack_At(opCodes, 0);
    toUpper(&op);

    if(strcmp(op, "") == 0)
    {
        Stack_free(&opCodes);
        return -1;
    }

    char* arg = (char*)Stack_At(opCodes, 1);
    remove_space(&arg);
    Stack* args = split(arg, ","); 

    int ret;

    if(strcmp(op, "ADD") == 0 || strcmp(op, "AND") == 0 || strcmp(op, "OR") == 0 || strcmp(op, "SLT") == 0 || strcmp(op, "SUB") == 0 ||
       strcmp(op, "XOR") == 0)
    {
        //args:  rd = 0, rs = 1, rt = 2
        ret = write_typeR_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 1)), to_register_code((char*)Stack_At(args, 2)), 
                           to_register_code((char*)Stack_At(args, 0)), 0);
    }
    else if(strcmp(op, "ADDI") == 0 || strcmp(op, "ORI") == 0)
    {
        //args: rt = 0, rs = 1, immediate = 2
        ret = write_typeI_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 1)), 
                           to_register_code((char*)Stack_At(args, 0)), atoi((char*)Stack_At(args, 2)));
    }
    else if(strcmp(op, "BEQ") == 0)
    {
        //args: rs = 0, rt = 1, offset = 2
        ret = write_typeI_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 0)), 
                           to_register_code((char*)Stack_At(args, 1)), atoi((char*)Stack_At(args, 2)));
    }
    else if(strcmp(op, "BGTZ") == 0 || strcmp(op, "BLEZ") == 0)
    {
        //args: rs = 0, offset = 1
        ret = write_typeI_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 0)), 0, atoi((char*)Stack_At(args, 1)));
    }
    else if(strcmp(op, "BNE") == 0)
    {
        //args: rs = 0, rt = 1, offset = 2
        ret = write_typeI_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 0)), 
                           to_register_code((char*)Stack_At(args, 1)), atoi((char*)Stack_At(args, 2)));
    }
    else if(strcmp(op, "DIV") == 0 || strcmp(op, "MULT") == 0)
    {
        //args: rs = 0, rt = 1
        ret = write_typeR_instr(to_operation_code(op), to_register_code((char*)Stack_At(args, 0)), to_register_code((char*)Stack_At(args, 1)), 0, 0);
    }
    else if(strcmp(op, "J") == 0 || strcmp(op, "JAL") == 0)
        ret = write_typeJ_instr(to_operation_code(op), atoi((char*)Stack_At(opCodes, 1)));
    else if(strcmp(op, "JR") == 0)
        ret = write_typeR_instr(to_operation_code(op), to_register_code((char*)Stack_At(opCodes, 1)), 0, 0, 0 /* hints */);
    else if(strcmp(op, "LUI") == 0)
    {
        //args: rt = 0, immediate = 1
        ret = write_typeI_instr(to_operation_code(op), 0, to_register_code((char*)Stack_At(args, 0)), atoi((char*)Stack_At(args, 1)));
    }
    else if(strcmp(op, "LW") == 0 || strcmp(op, "SW") == 0)
    {
        //args: rt = 0, offset(base) = 1
        Stack* offbase = split((char*)Stack_At(args, 1), "()"); // offset = 0, base = 1
        ret = write_typeI_instr(to_operation_code(op), to_register_code((char*)Stack_At(offbase, 1)), to_register_code((char*)Stack_At(args, 0)), atoi((char*)Stack_At(offbase, 0)));
        Stack_free(&offbase);
    }
    else if(strcmp(op, "MFHI") == 0 || strcmp(op, "MFLO") == 0)
        ret = write_typeR_instr(to_operation_code(op), 0, 0, to_register_code((char*)Stack_At(opCodes, 1)), 0);
    else if(strcmp(op, "NOP") == 0)
        ret = 0;
    else if(strcmp(op, "ROTR") == 0 || strcmp(op, "SLL") == 0 || strcmp(op, "SRL") == 0)
    {
        //args: rd = 0, rt = 1, sa = 2
        ret = write_typeR_instr(to_operation_code(op), strcmp(op, "ROTR") == 0, to_register_code((char*)Stack_At(args, 1)), 
                           to_register_code((char*)Stack_At(args, 0)), atoi((char*)Stack_At(args, 2)));
    }
    else if(strcmp(op, "SYSCALL") == 0)
    {
        ret = 0xc;
    }

    Stack_free(&args);
    Stack_free(&opCodes);

    return ret;
}

char* translate_op_asm(unsigned int op)
{
    if(op == 0)
        return strdup("NOP");
    
    char* ret = NULL;
    char opCode = 0, rs = 0, rt = 0, rd = 0, sa = 0;
    int index = 0;
    short immediate = 0;
    if((op & 0xfc000000) == 0)
    {
        opCode = op & 0x3f;
        rs = (op >> 21) & 0x1f;
        rt = (op >> 16) & 0x1f;
        rd = (op >> 11) & 0x1f;
        sa = (op >> 6) & 0x1f;

        if(opCode == 0x20)
            ret = read_typeR_instr("ADD ", rd, rs, rt |= 0x40);
        else if(opCode == 0x24)
            ret = read_typeR_instr("AND ", rd, rs, rt |= 0x40);
        else if(opCode == 0x1a)
            ret = read_typeR_instr("DIV ", rs, rt |= 0x40, 0x80);
        else if(opCode == 0x08)
            ret = read_typeR_instr("JR ", rs |= 0x40, 0x80, 0x80);
        else if(opCode == 0x10)
            ret = read_typeR_instr("MFHI ", rd |= 0x40, 0x80, 0x80);  
        else if(opCode == 0x12)
            ret = read_typeR_instr("MFLO ", rd |= 0x40, 0x80, 0x80);
        else if(opCode == 0x18)
            ret = read_typeR_instr("MULT ", rs, rt |= 0x40, 0x80);
        else if(opCode == 0x25)
            ret = read_typeR_instr("OR ", rd, rs, rt |= 0x40);
        else if(opCode == 0x2 && (op & 0x200000) == 0x200000)
            ret = read_typeR_instr("RORT ", rd, rt, sa |= 0x60);
        else if(opCode == 0x0)
            ret = read_typeR_instr("SLL ", rd, rt, sa |= 0x60);
        else if(opCode == 0x2a)
            ret = read_typeR_instr("SLT ", rd, rs, rt |= 0x40);
        else if(opCode == 0x2 && (op & 0x200000) == 0)
            ret = read_typeR_instr("SRL ", rd, rs, sa |= 0x60);
        else if(opCode == 0x22)
            ret = read_typeR_instr("SUB ", rd, rs, rt |= 0x40);
        else if(opCode == 0xc)
            return strdup("SYSCALL");
        else if(opCode == 0x26)
            ret = read_typeR_instr("XOR ", rd, rs, rt |= 0x40);
        else
            ret = strdup("UNKNOW R TYPE INSTR"); 
    }
    else
    {
        opCode = (op & 0xfc000000) >> 26;
        if(opCode == 0x2 || opCode == 0x3)
            index = op & 0x3ffffff;
        else
        {
            rs = (op >> 21) & 0x1f;
            rt = (op >> 16) & 0x1f;
            immediate = op & 0xffff;
        }     
        if(opCode == 0x8)
            ret = read_typeI_instr("ADDI ", rt, rs, immediate);
        else if(opCode == 0x4)
            ret = read_typeI_instr("BEQ ", rs, rt, immediate);
        else if(opCode == 0x7)
            ret = read_typeI_instr("BGTZ ", rs, 0x80, immediate);
        else if(opCode == 0x6)
            ret = read_typeI_instr("BLEZ ", rs, 0x80, immediate);
        else if(opCode == 0x5)
            ret = read_typeI_instr("BNE ", rs, rt, immediate);
        else if(opCode == 0x2)
            ret = read_typeJ_instr("J ", index);
        else if(opCode == 0x3)
            ret = read_typeJ_instr("JAL ", index);
        else if(opCode == 0xf)
            ret = read_typeI_instr("LUI ", rt, 0x80, immediate);
        else if(opCode == 0x23)
            ret = read_typeI_instr("LW ", rt, rs, immediate);
        else if(opCode == 0x2b)
            ret = read_typeI_instr("SW ", rt, rs, immediate);
        else if(opCode == 0xd)
            ret = read_typeI_instr("ORI ", rt, rs, immediate);
        else
            ret = strdup("UNKNOW I/J TYPE INSTR");   
    }    

    return ret;   
}