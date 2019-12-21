#include "CPU.h"

void CPU_Init(CPU* cpu)
{
    InitMemory(&(cpu->memory));

    Registers_Init(&(cpu->registers));

    cpu->running = 1;
}

void CPU_Main(CPU* cpu, char pas, unsigned char silent)
{
    while(cpu->running)
    {
        unsigned int code = readCode(&(cpu->memory), cpu->registers.PC);
        if(code == -1)
        {
            cpu->running = 0;
            continue;
        }

        char* asm_str = translate_op_asm(code);

        if(!silent)
            printf("Processing instruction:\n%08x  %s\n", code, asm_str);
            
        free(asm_str);
        CPU_Execute(cpu);
        
        if(pas)
        {
            char c = 0;
            do
            {
                puts("*** [r]: display registers; [m]: memory; [t]: text segment; [c]: continue");
                c = getchar();
                flush();
                switch(c)
                {
                case 'r':
                    printf("----- Registers ------\n");
                    Registers_Print(&(cpu->registers));
                    break;
                case 'm':
                    printf("----- Stack ------\n");
                    MMU_Print(&(cpu->memory), 0, 16*5);
                    break;
                case 't':
                    print_text_segment_arrow(&(cpu->memory), cpu->registers.PC);
                    break;
                }
            } while (c != 'c' && c != 0xa);
            
        }       
    }
    puts("");
    puts("*** Final register states: ***");
    Registers_Print(&(cpu->registers));
    puts("*** Final memory states: ***");
    MMU_Print(&(cpu->memory), 0, 16*5);
}

void Execute_SYSCALL(CPU* cpu)
{
    //printf("Executing SYSCALL => %d\n", cpu->registers.reg[2]);
    //printf("> ");
    switch(cpu->registers.reg[2])
    {
    case 1:
        printf("%d", cpu->registers.reg[4]);
        break;
    case 4:
        for(Address i = cpu->registers.reg[4]; cpu->memory.ROM[i] != 0; i++)
            printf("%c", cpu->memory.ROM[i]);
        break;
    case 5:
        scanf("%d", &cpu->registers.reg[2]);
        break;
    case 8:
        flush();
        size_t size = cpu->registers.reg[4] + 1;
        char* tmp = (char*)malloc(size);
        fgets(tmp, size - 1, stdin);
        for(unsigned int i = 0; i < size; i++)
            writeByte(&cpu->memory, cpu->registers.reg[4] + i, tmp[i]);
        writeByte(&cpu->memory, cpu->registers.reg[4] + size, 0);
        break;
    case 10:
        cpu->running = 0;
        break;
    default:
        printf("Unknown SYSCALL !\n");
        break;
    }
}

void CPU_Execute(CPU* cpu)
{
    unsigned int instr = readCode(&(cpu->memory), cpu->registers.PC);
    cpu->registers.PC += 4;

    if((instr & 0xfc000000) == 0)
    {
        char opCode = instr & 0x3f;
        char rs = (instr >> 21) & 0x1f;
        char rt = (instr >> 16) & 0x1f;
        char rd = (instr >> 11) & 0x1f;
        char sa = (instr >> 6) & 0x1f;

        if(opCode == 0x20)
            cpu->registers.reg[rd] = cpu->registers.reg[rs] + cpu->registers.reg[rt];
        else if(opCode == 0x24)
            cpu->registers.reg[rd] = cpu->registers.reg[rs] & cpu->registers.reg[rt];
        else if(opCode == 0x1a)
        {
            cpu->registers.LO = cpu->registers.reg[rs] / cpu->registers.reg[rt];
            cpu->registers.HI = cpu->registers.reg[rs] % cpu->registers.reg[rt];
        }
        else if(opCode == 0x08)
            cpu->registers.PC = cpu->registers.reg[rs];
        else if(opCode == 0x10)
            cpu->registers.reg[rd] = cpu->registers.HI;  
        else if(opCode == 0x12)
            cpu->registers.reg[rd] = cpu->registers.LO;  
        else if(opCode == 0x18)
        {
            long l = cpu->registers.reg[rs] * cpu->registers.reg[rt];
            cpu->registers.LO = (l & 0xffffffff);
            cpu->registers.HI = ((l & 0xffffffff00000000) >> 32);
        }
        else if(opCode == 0x25)
            cpu->registers.reg[rd] = cpu->registers.reg[rs] | cpu->registers.reg[rt];
        else if(opCode == 0x2 && (instr & 0x200000) == 0x200000)
            cpu->registers.reg[rd] = ((cpu->registers.reg[rt] >> sa) | (cpu->registers.reg[rt] << (32 - sa)));
        else if(opCode == 0x0)
            cpu->registers.reg[rd] = (cpu->registers.reg[rt] << sa);
        else if(opCode == 0x2a)
            cpu->registers.reg[rd] = (int)cpu->registers.reg[rs] < (int)cpu->registers.reg[rt];
        else if(opCode == 0x2 && (instr & 0x200000) == 0)
            cpu->registers.reg[rd] = (cpu->registers.reg[rt] >> sa);
        else if(opCode == 0x22)
            cpu->registers.reg[rd] = cpu->registers.reg[rs] - cpu->registers.reg[rt];
        else if(opCode == 0xc)
            Execute_SYSCALL(cpu); //SYSCALL
        else if(opCode == 0x26)
            cpu->registers.reg[rd] = cpu->registers.reg[rs] ^ cpu->registers.reg[rt];
    }
    else
    {
        char opCode = (instr & 0xfc000000) >> 26;
        char index, rs, rt;
        short immediate;
        if(opCode == 0x2 || opCode == 0x3)
            index = instr & 0x3ffffff;
        else
        {
            rs = (instr >> 21) & 0x1f;
            rt = (instr >> 16) & 0x1f;
            immediate = instr & 0xffff;
        }     
        if(opCode == 0x8)
            cpu->registers.reg[rt] = cpu->registers.reg[rs] + immediate;
        else if(opCode == 0x4)
        {
            if(cpu->registers.reg[rs] == cpu->registers.reg[rt])
                cpu->registers.PC = cpu->registers.PC + (immediate << 2);
        }
        else if(opCode == 0x7)
        {
            if(cpu->registers.reg[rs] > 0)
                cpu->registers.PC = cpu->registers.PC + (immediate << 2);
        }
        else if(opCode == 0x6)
        {
            if(cpu->registers.reg[rs] <= 0)
                cpu->registers.PC = cpu->registers.PC + (immediate << 2);
        }
        else if(opCode == 0x5)
        {
            if(cpu->registers.reg[rs] != cpu->registers.reg[rt])
                cpu->registers.PC = cpu->registers.PC + (immediate << 2);
        }
        else if(opCode == 0x2)
            cpu->registers.PC = (index << 2);
        else if(opCode == 0x3)
        {
            cpu->registers.reg[31] = cpu->registers.PC;
            cpu->registers.PC = (index << 2);
        }
        else if(opCode == 0xf)
            cpu->registers.reg[rt] = (immediate << 16);
        else if(opCode == 0x23)
            cpu->registers.reg[rt] = readDWord(&(cpu->memory), cpu->registers.reg[rs] + immediate);
        else if(opCode == 0x2b)
            writeDWord(&(cpu->memory), cpu->registers.reg[rs] + immediate, cpu->registers.reg[rt]);  
        else if(opCode == 0xd)
            cpu->registers.reg[rt] = cpu->registers.reg[rs] | immediate;
    }
}