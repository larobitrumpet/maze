#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "maze.h"
#include "algorithms.h"

void must_init(bool test, const char *description)
{
    if (test) return;

    printf("Couldn't initialize %s\n", description);
}

int BUFFER_W;
int BUFFER_H;

float DISP_SCALE_W;
float DISP_SCALE_H;
float DISP_W;
float DISP_H;
float DISP_W_OFFSET;
float DISP_H_OFFSET;

ALLEGRO_DISPLAY *disp;
ALLEGRO_BITMAP *buffer;

void disp_init()
{
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW + ALLEGRO_RESIZABLE);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);

    disp = al_create_display(BUFFER_W, BUFFER_H);
    must_init(disp ,"display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    float BUFFER_ASPECT = (float)BUFFER_W / (float)BUFFER_H;
    float D_W = (float)al_get_display_width(disp);
    float D_H = (float)al_get_display_height(disp);
    float DISP_ASPECT = D_W / D_H;

    DISP_SCALE_W = D_W / BUFFER_W;
    DISP_SCALE_H = D_H / BUFFER_H;
    
    if (DISP_ASPECT < BUFFER_ASPECT)
    {
        DISP_W = BUFFER_W * DISP_SCALE_W;
        DISP_H = BUFFER_H * DISP_SCALE_W;

        float DD_H = BUFFER_H * DISP_SCALE_H;

        DISP_H_OFFSET = (DD_H - DISP_H) / 2;
        DISP_W_OFFSET = 0;
    }
    else if (DISP_ASPECT > BUFFER_ASPECT)
    {
        DISP_W = BUFFER_W * DISP_SCALE_H;
        DISP_H = BUFFER_H * DISP_SCALE_H;

        float DD_W = BUFFER_W * DISP_SCALE_W;

        DISP_W_OFFSET = (DD_W - DISP_W) / 2;
        DISP_H_OFFSET = 0;
    }
    else
    {
        DISP_W = BUFFER_W * DISP_SCALE_W;
        DISP_H = BUFFER_H * DISP_SCALE_H;

        DISP_W_OFFSET = 0;
        DISP_H_OFFSET = 0;
    }

    al_set_target_backbuffer(disp);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, DISP_W_OFFSET, DISP_H_OFFSET, DISP_W, DISP_H, 0);

    al_flip_display();
}

#define KEY_SEEN     1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch (event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

MAZE maze;

#define TILE_W 8
#define TILE_H 8

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;
    ALLEGRO_BITMAP* tiles[3][16];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("maze_tiles.png");
    must_init(sprites._sheet, "maze tiles");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            sprites.tiles[i][j] = sprite_grab(i * TILE_W, j * TILE_H, TILE_W, TILE_H);
        }
    }
}

void sprites_deinit()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            al_destroy_bitmap(sprites.tiles[i][j]);
        }
    }

    al_destroy_bitmap(sprites._sheet);
}

void draw_maze()
{
    for (int y = 0; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            al_draw_bitmap(sprites.tiles[get_maze_type_value(maze, x, y)][get_maze_passage_value(maze, x, y)], x * TILE_W, y * TILE_H, 0);
        }
    }
}

void draw()
{
    disp_pre_draw();

    // draw code
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_maze();

    disp_post_draw();
}

void update_maze_display()
{
    //print_maze(maze);
    draw();
}

int main()
{
    srand((unsigned)time(NULL));
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;

    int redraw = true;
    must_init(al_init(), "Allegro");
    must_init(al_init_image_addon(), "image");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    // init
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);

    int maze_w = 60;
    int maze_h = 30;
    //int maze_w = 8;
    //int maze_h = 8;

    BUFFER_W = maze_w * TILE_W;
    BUFFER_H = maze_h * TILE_H;

    disp_init();

    sprites_init();

    keyboard_init();

    timer = al_create_timer(1.0 / 60);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // setup_scene();
    maze = construct_maze(maze_w, maze_h);
    //recursive_backtracking(maze);
    //eller(maze);
    //kruskal(maze);
    //prim(maze);
    recursive_division(maze);

    bool done = false;
    al_start_timer(timer);
    while(true)
    {
        ALLEGRO_EVENT event;

        al_wait_for_event(queue, &event);
        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // game logic
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(disp);
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ESCAPE:
                        done = true;
                        break;
                }
                break;
        }

        if (done)
        {
            break;
        }

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            // redraw code
            draw();
            redraw = false;
        }
    }

    // deinit
    sprites_deinit();
    disp_deinit();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
