#include "Utils.h"
#include "CPU.h"
#include "Regex.h"
#include "Label.h"

Stack* remplace_label_with_address(Stack* text_section, Stack* labels)
{
    Stack* ret = Stack_Init();
    Address tmpPC = TEXT_ADDRESS;

    Stack* tmp = text_section;
    while(tmp != NULL)
    {
        char* line = strdup((char*)tmp->value);

        if(strchr(line, ':') != NULL)
        {
            tmp = tmp->next;
            continue;
        }

        Stack* tmp2 = labels;
        char needChange = 0;
        while(tmp2 != NULL)
        {
            char* offset = NULL;
            if((offset = strstr(line, ((Label*)tmp2->value)->name)) != NULL)
            {
                char* newline = (char*)calloc(strlen(line) + 1, 1);
                strncpy(newline, line, offset - line);

                Address addr = ((Label*)tmp2->value)->addr;
                if(((Label*)tmp2->value)->section == TEXT)
                {
                    addr = (((addr - tmpPC) / 4) & 0x3ffffff) - 1;
                    printf("Relative Address of %s is at %x, current PC = %x\n", ((Label*)tmp2->value)->name, addr, tmpPC);
                }

                if(addr & 0x2000000)
                    sprintf(newline + (offset - line), "-%d", addr - 1 ^ 0x3ffffff);
                else
                    sprintf(newline + (offset - line), "%d", addr);

                Stack_Insert(&ret, newline);
                needChange = 1;
                break;
            }
            tmp2 = tmp2->next;            
        }
        if(!needChange)
            Stack_Insert(&ret, line);

        tmpPC += 4;
        
        tmp = tmp->next;
    }

    return ret;
}

Stack* asm_to_hex(Stack* assembly, char pas, FILE* outfile)
{
    puts("");
    char skip = 0;
    Stack* stack_tmp = assembly;
    Stack* ret = Stack_Init();
    while(stack_tmp != NULL)
    {
        char* tmp = strdup((char*)stack_tmp->value);
        trim(&tmp);
        if(strcmp(tmp, "") == 0)
        {
            free(tmp);
            continue;
        }
        unsigned int translate = translate_asm_op(tmp);
        if(translate != -1)
        {
            if(pas && !skip)
            {
                printf("Assembling line %s\n\nHexadecimal instruction code:\n%08x\n", tmp, translate);
                int c = 0, buffer;
                do
                {
                    puts("*** [entry]: go to next instruction; [s] skip to execution:");
                    c = getchar();
                    if(c != '\n')
                        flush();
                } while(c != 's' && c != '\n');
                if(c == 's')
                    skip = 1;
            }
            if(outfile != NULL)
                fprintf(outfile, "%08x\n", translate);
            Stack_Insert(&ret, (void*)translate);
        }

        free(tmp);
        stack_tmp = stack_tmp->next;
    } 
    return ret;
}

int main(int argc, char** argv)
{
    FILE* outfile = NULL;
    char* outPath = NULL;
    char pas = 0;
    char silent = 0;
    
    if(argc < 2)
    {
        printf("Usage: %s input [output] [-pas / -silent]\n", argv[0]);
        return -1;
    }

    if(argc == 3)
    {
        if(strcmp("-pas", argv[2]) == 0)
            pas = 1;
        else if(strcmp("-silent", argv[2]) == 0)
            silent = 1;
        else
            outPath = argv[2];
    }
    else
    {
        outPath = argv[2];
        if(strcmp("-pas", argv[3]) == 0)
            pas = 1;
        else if(strcmp("-silent", argv[3]) == 0)
            silent = 1;
    }
    

    printf("Assembly file : %s\n", outPath);
    if(outPath != NULL)
    {
        printf("Output will be written in : %s\n\n", outPath);
        outfile = fopen(outPath, "w");
    }

    FILE* infile = fopen(argv[1], "r");

    if(infile == NULL)
        return 1;

    size_t read = 0, len = 0;
    char* line = NULL;
    unsigned int i = 1;

    fseek(infile, 0, SEEK_END);
    size_t fileSize = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    char* file_str = (char*)malloc(fileSize);
    fread(file_str, 1, fileSize, infile);

    char* no_comment = remove_comments(file_str);

    if(line != NULL)
        free(line);   

    CPU cpu;
    CPU_Init(&cpu);

    Stack* data = getSectionContent(no_comment, ".data");
    Stack* text = getSectionContent(no_comment, ".text");

    Stack* labels = Stack_Init();
    add_label_from_section(&labels, data, DATA, &cpu);
    add_label_from_section(&labels, text, TEXT, &cpu);

    Stack* assembly = remplace_label_with_address(text, labels);
    Stack* hexs = asm_to_hex(assembly, pas, outfile);
    writeCode(&(cpu.memory), hexs);

    puts("--------DATA Memory--------");
    MMU_Print(&(cpu.memory), DATA_ADDRESS, 0x40);
    puts("--------CODE Memory--------");
    MMU_Print(&(cpu.memory), TEXT_ADDRESS, 0x40);

    puts("*** Text segment loaded - Ready to execute ***");
    print_text_segment(&(cpu.memory));
    puts("\n*** Starting program execution ***\nPress Enter to continue");
    getchar();
    //Stack_clear(&code);

    CPU_Main(&cpu, pas, silent);

    fclose(infile);

    if(outfile != NULL)
        fclose(outfile);

    return 0;
}