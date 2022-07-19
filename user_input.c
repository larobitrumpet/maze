#include "user_input.h"

static unsigned char validate_integer(char *a)
{
    int len = strlen(a);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(a[i]))
            return 1;
    }
    return 0;
}

int user_get_integer(char* prompt)
{
    while (1)
    {
        char buffer[USER_INPUT_BUFF_SIZE];
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            buffer[strlen(buffer) - 1] = '\0';
            if (validate_integer(buffer) == 0)
                return atoi(buffer);
        }
        printf("Please enter a valid integer\n");
    }
}

void user_get_maze_dimentions(int* width, int* height)
{
    printf("Enter the dimentions of the maze:\n");
    *width = user_get_integer("  Width: ");
    *height = user_get_integer("  Height: ");
}

void user_get_algorithm(int* al)
{
    printf("Select maze generation algorithm:\n");
    printf("  0: Recursive Backtracker\n");
    printf("  1: Eller's Algorithm\n");
    printf("  2: Kruskal's Algorithm\n");
    printf("  3: Prim's Algorithm\n");
    printf("  4: Recursive Division\n");
    printf("  5: Aldous-Broder\n");
    printf("  6: Wilson's Algorithm\n");
    printf("  7: Hunt-and-Kill\n");
    while (1)
    {
        *al = user_get_integer("Enter maze number: ");
        if (*al >= 0 && *al < 8)
            break;
        printf("Please enter a valid maze number\n");
    }
}
