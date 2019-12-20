#include "Stack.h"

Stack* Stack_Init()
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->next = NULL;
    s->value = NULL;
    return s;
}

void Stack_Insert(Stack** stack, void* value)
{
    if(Stack_isEmpty(*stack))
    {
        (*stack)->value = value;
        return;
    }

    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->value = value;
    s->next = NULL;

    Stack* tmp = *stack;
    while(tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = s;
}

char Stack_isEmpty(Stack* stack)
{
    return stack->value == NULL && stack->next == NULL;
}

void* Stack_At(Stack* stack, unsigned int i)
{
    Stack* tmp = stack;
    unsigned int j = 0;
    while(tmp != NULL)
    {
        if(j == i)
            return tmp->value;
        j++;
        tmp = tmp->next;
    }
    return NULL;
}

void Stack_clear(Stack** stack)
{
    Stack* tmp = *stack;
    while(tmp != NULL)
    {
        Stack* next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

void Stack_free(Stack** stack)
{
    Stack* tmp = *stack;
    while(tmp != NULL)
    {
        Stack* next = tmp->next;
        if(tmp->value != NULL)
        {
            free(tmp->value);
            tmp->value = NULL;
        }
        free(tmp);
        tmp = next;
    }
}