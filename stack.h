typedef struct POINT
{
    int x;
    int y;
} POINT;

typedef struct STACKNODE
{
    POINT value;
    struct STACKNODE* next;
} STACKNODE;

typedef struct STACK
{
    STACKNODE* head;
} STACK;

static STACKNODE* construct_stacknode();
static inline void deconstruct_stacknode(STACKNODE* node);
STACK* construct_stack();
void deconstruct_stack(STACK* stack);
STACK* stack_push(STACK* stack, POINT value);
POINT stack_pop(STACK* stack);
POINT stack_peak(STACK* stack);
unsigned char stack_is_empty(STACK* stack);
