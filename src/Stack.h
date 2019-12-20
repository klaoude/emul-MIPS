#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Stack
{
    void* value;
    struct Stack* next;
} Stack;

Stack* Stack_Init();
void Stack_Insert(Stack** stack, void* value);
char Stack_isEmpty(Stack* stack);
void* Stack_At(Stack* stack, unsigned int i);
void Stack_free(Stack** stack);
void Stack_clear(Stack** stack);

#endif