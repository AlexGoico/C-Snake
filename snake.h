#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdbool.h>

#include "sdl_system.h"

extern Color wht;
extern Color blk;
extern Color red;
extern Color grn;

enum Direction { UP, RIGHT, DOWN, LEFT };

// Snake is measured in game Grid Units.
typedef struct Pos_Block
{
    int x, y;
    struct Pos_Block* next;
    struct Pos_Block* prev;
} Pos_Block;

typedef struct Snake
{
    Pos_Block* body;
    Pos_Block* head;
    Pos_Block* tail;
    int width, height, size;
    enum Direction dir;
    bool alive;
} Snake;

enum Obj { EMPTY, PLAYER, APPLE };

void init();
void snake_init();
void cleanup();

bool process_input();

void assign_position(Pos_Block* block, int x, int y);
SDL_Rect to_render_units(Pos_Block* pos, Snake* snk);

void paint_map(Pos_Block* pos, enum Obj o);

void move(Snake* snk);
void rotate(Snake* snk);

void gen_apple();
bool ate_apple(Snake* snk);
void clear_apple(Snake* snk);

bool killed(Snake* snk);

#endif 
