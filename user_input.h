#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef USER_INPUT_H
#define USER_INPUT_H

#define USER_INPUT_BUFF_SIZE 16

static unsigned char validate_integer(char *a);
int user_get_integer(char* prompt);
void user_get_maze_dimentions(int* width, int* height);
void user_get_algorithm(int* al);

#endif
