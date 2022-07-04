#include "stack.h"

static STACKNODE* construct_stacknode()
{
    STACKNODE* stacknode = (STACKNODE*)malloc(sizeof(STACKNODE));
    stacknode->value.x = -1;
    stacknode->value.y = -1;
    stacknode->next = NULL;
    return stacknode;
}

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
    STACKNODE* node = stack->head;
    while (node != NULL)
    {
        STACKNODE* nextnode = node->next;
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
    {
        POINT p;
        p.x = -1;
        p.y = -1;
        return p;
    }
    STACKNODE* node = stack->head;
    POINT value = node->value;
    stack->head = node->next;
    deconstruct_stacknode(node);
    return value;
}

POINT stack_peak(STACK* stack)
{
    if (stack->head == NULL)
    {
        POINT p;
        p.x = -1;
        p.y = -1;
        return p;
    }
    return stack->head->value;
}

unsigned char stack_is_empty(STACK* stack)
{
    return (stack->head == NULL);
}
