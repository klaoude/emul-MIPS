#include "Utils.h"
#include "CPU.h"

int main(int argc, char** argv)
{
    FILE* outfile = NULL;
    char* outPath = NULL;
    char pas = 0;
    
    if(argc < 2)
    {
        printf("Usage: %s input [output] [-pas]\n", argv[0]);
        return -1;
    }

    if(argc == 3)
    {
        if(strcmp("-pas", argv[2]) == 0)
            pas = 1;
        else
            outPath = argv[2];
    }
    else
    {
        outPath = argv[2];
        if(strcmp("-pas", argv[3]) == 0)
            pas = 1;
    }
    

    printf("Assembly file : %s\n", argv[2]);
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
    Stack* code = Stack_Init();
    char skip = 0;
    unsigned int i = 1;

    while((read = getline(&line, &len, infile)) != -1)
    {
        char* tmp = strdup(line);
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
                printf("Assembling line %d:\n%s\n\nHexadecimal instruction code:\n%08x\n", i, tmp, translate);
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
            Stack_Insert(&code, (void*)translate);
        }

        free(tmp);
    } 

    if(line != NULL)
        free(line);   

    CPU cpu;
    CPU_Init(&cpu);
    writeCode(&(cpu.memory), code);
    puts("*** Text segment loaded - Ready to execute ***");
    print_text_segment(&(cpu.memory));
    puts("\n*** Starting program execution ***\nPress Enter to continue");
    getchar();
    Stack_clear(&code);

    CPU_Main(&cpu, pas);

    fclose(infile);

    if(outfile != NULL)
        fclose(outfile);

    return 0;
}