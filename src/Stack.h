#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

//Chained list structure
typedef struct Stack
{
    void* value;
    struct Stack* next;
} Stack;

//function to alloc a Stack
Stack* Stack_Init();

//Insert an element on top of the stack
void Stack_Insert(Stack** stack, void* value);

//Check if a Stack is empty
char Stack_isEmpty(Stack* stack);

//Return the i-th value on the stack
void* Stack_At(Stack* stack, unsigned int i);

//Free value and stack for all the stack
void Stack_free(Stack** stack);

//Free stack for all the stack
void Stack_clear(Stack** stack);

//Return the size of the stack
size_t Stack_length(Stack* stack);

#endif