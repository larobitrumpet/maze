#ifndef MAZE_H
#define MAZE_H

typedef struct MAZE
{
    unsigned char* values;
    int* pos_x;
    int* pos_y;
    int width;
    int height;
} MAZE;

MAZE construct_maze(int width, int height);
void deconstruct_maze(MAZE maze);
static inline unsigned char get_maze_value(MAZE maze, int x, int y);
static inline void set_maze_value(MAZE maze, int x, int y, unsigned char val);
static inline unsigned char set_bit(unsigned char num, unsigned char bit);
static inline unsigned char clear_bit(unsigned char num, unsigned char bit);
static inline void set_maze_bit(MAZE maze, int x, int y, unsigned char bit);
static inline void clear_maze_bit(MAZE, int x, int y, unsigned char bit);
void maze_set_up_open(MAZE maze, int x, int y);
void maze_set_up_close(MAZE maze, int x, int y);
void maze_set_left_open(MAZE maze, int x, int y);
void maze_set_left_close(MAZE maze, int x, int y);
void maze_set_down_open(MAZE maze, int x, int y);
void maze_set_down_close(MAZE maze, int x, int y);
void maze_set_right_open(MAZE maze, int x, int y);
void maze_set_right_close(MAZE maze, int x, int y);
static inline void maze_set_pos_value(MAZE maze, int x, int y);
static inline void maze_clear_pose_value(MAZE maze, int x, int y);
void maze_set_pos(MAZE maze, int x, int y);
void maze_set_special_value(MAZE maze, int x, int y);
void maze_clear_special_value(MAZE maze, int x, int y);
unsigned char get_maze_passage_value(MAZE maze, int x, int y);
unsigned char get_maze_type_value(MAZE maze, int x, int y);

#endif
