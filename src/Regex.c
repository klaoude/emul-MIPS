#include "Regex.h"

Stack* regex_match(char* str, char* reg)
{
    Stack* found = Stack_Init();
    regex_t preg;
    int err = regcomp(&preg, reg, REG_EXTENDED);
    if(err)
        return NULL;

    int match;
    size_t nmatch;
    regmatch_t* pmatch = NULL;
    nmatch = preg.re_nsub + 1; 
    pmatch = (regmatch_t*)malloc(sizeof(regmatch_t) * nmatch);
    if(pmatch)
    {
        match = regexec(&preg, str, nmatch, pmatch, 0);
        printf("match = %d, nmatch = %d\n", match, nmatch);
        regfree(&preg);
        if(match == 0)
        {            
            for(int i = 1; i < nmatch; i++)
            {
                int start = pmatch[i].rm_so;
                int end = pmatch[i].rm_eo;
                size_t size = end - start;
                char* tmp = (char*)malloc(size + 1);
                if(tmp)
                {
                    strncpy(tmp, &str[start], size);
                    tmp[size] = 0;
                    if(*tmp != 0)
                    {
                      printf("Match(%d) : %s\n", i, tmp);
                      Stack_Insert(&found, (void*)tmp);
                    }                    
                }                
            }
        }
        else
            return NULL;
        
        free(pmatch);
    }
    else
        return NULL;

    return found;
}