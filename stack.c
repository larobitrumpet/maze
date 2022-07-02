#include <stdlib.h>
#include "stack.h"

static inline void deconstruct_stacknode(STACKNODE* node)
{
    free(node);
}

STACK* construct_stack()
{
    STACK* stack = (STACK*)malloc(sizeof(STACK));
    stack->head = NULL;
    return stack;
}

void deconstruct_stack(STACK* stack)
{
    node = stack->head
    while (node != NULL)
    {
        STACK* nextnode = node->next;
        deconstruct_stacknode(node);
        node = nextnode;
    }
    free(stack);
}

STACK* stack_push(STACK* stack, POINT value)
{
    STACKNODE* newnode = construct_stacknode();
    newnode->value = value;
    newnode->next = stack->head;
    stack->head = newnode;
    return stack;
}

POINT stack_pop(STACK* stack)
{
    if (stack->head == NULL)
        return NULL;
    STACKNODE* node = stack->head;
    POINT value = node->value;
    stack->head = node->next;
    deconstruct_stacknode(node);
    return value;
}

POINT stack_peak(STACK* stack)
{
    if (stack->head == NULL)
        return NULL;
    return stack->head->value;
}

unsigned char stack_is_empty(STACK* stack)
{
    return (stack->head == NULL)
}
