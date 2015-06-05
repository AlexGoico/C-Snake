#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake.h"

static enum Obj map[CELLS_WIDE][CELLS_HIGH];   // initialized to 0 since static

Snake snake;

int main()
{
    init();

    //double mspf = 1000 / fps;
    Uint32 prev = SDL_GetTicks(), cur;

    bool quit = false;
    while (snake.alive && !quit )
    {
        // Input
        quit = process_input();

        // Logic
        cur = SDL_GetTicks();
        if (cur >= prev + 50)   // Move every 50ms
        {
            // Deletes tail by painting it white.
            fill_and_restore(wht, to_render_units(snake.tail, &snake));
            prev = cur;
            move(&snake);
        }

        // Render head and show updated buffer
        fill_rect(to_render_units(snake.head, &snake));
        buffer_flip();

        Uint32 wait_time = prev + 50 - SDL_GetTicks();
        if (wait_time > 0)
            SDL_Delay(wait_time);
    }
    SDL_Delay(2000);

    cleanup();
}

void init()
{
    srand(time(NULL));
    sdl_system_init();
    snake_init();
    gen_apple();
}

void snake_init()
{
    Pos_Block* head = malloc(1 * sizeof(Pos_Block));
    assign_position(head, 2, 5);
    head->prev = NULL;

    snake = (struct Snake) { head, head, head, 1, 1, 4, DOWN, true};
    fill_rect(to_render_units(head, &snake));

    Pos_Block* iter = head;
    for (int i = 4; i != 1; i--)
    {
        Pos_Block* temp = malloc(1 * sizeof(Pos_Block));
        assign_position(temp, 2, i);
        temp->prev = iter;
        iter->next = temp;
        iter = temp;
        fill_rect(to_render_units(temp, &snake));
        map[2][i] = PLAYER;
    }
    snake.tail = iter;
    iter->next = NULL;
}

void cleanup()
{
    for (Pos_Block* iter = snake.head; iter; iter = iter->next)
        free(iter);
    sdl_system_cleanup();
}

bool process_input()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return true;
        else if (e.key.keysym.sym == SDLK_UP)
        {
            if (snake.dir != DOWN)
                snake.dir = UP;
        }
        else if (e.key.keysym.sym == SDLK_LEFT)
        {
            if (snake.dir != RIGHT)
                snake.dir = LEFT;
        }
        else if (e.key.keysym.sym == SDLK_DOWN)
        {
            if (snake.dir != UP)
                snake.dir = DOWN;
        }
        else if (e.key.keysym.sym == SDLK_RIGHT)
        {
            if (snake.dir != LEFT)
                snake.dir = RIGHT;
        }
    }

    return false;
}

void assign_position(Pos_Block* block, int x, int y)
{
    block->x = x;
    block->y = y;
}

SDL_Rect to_render_units(Pos_Block* pos, Snake* snk)
{
    return (SDL_Rect) { pos->x * CELL_WIDTH, pos->y * CELL_HEIGHT,
            snk->width * CELL_WIDTH,
            snk->height * CELL_HEIGHT};
}

// Paints the map. The map acts as a collision check center for O(1) checks
// against other entities.
void paint_map(Pos_Block* pos, enum Obj o)
{
    int x = pos->x;
    int y = pos->y;
    map[x][y] = o;
}

void rotate(Snake* snk)
{
    paint_map(snk->tail, EMPTY);

    snk->tail->prev->next = NULL; // Second to last points to nothing
    snk->tail->next = snk->head;  // Last points to head
    snk->head->prev = snk->tail;  // head points to last
    snk->head = snk->tail;        // last becomes head
    snk->tail = snk->tail->prev;  // second to last becomes last
    snk->head->prev = NULL;    // old_last stops pointing to second to last
                               // assign tail head coordinates for defered update
    assign_position(snk->head, snk->head->next->x, snk->head->next->y);
}

void move(Snake* snk)
{
    rotate(snk);

    switch (snk->dir)
    {
        case UP:    snk->head->y--; break;
        case LEFT:  snk->head->x--; break;
        case DOWN:  snk->head->y++; break;
        case RIGHT: snk->head->x++; break;
    }

    Pos_Block* new_tail = NULL;
    if (ate_apple(snk))
    {
        clear_apple(snk);
        gen_apple();
        new_tail = malloc(1 * sizeof(Pos_Block));
        new_tail->next = NULL;
        assign_position(new_tail, snk->tail->x, snk->tail->y);
    }

    if (new_tail)
    {
        snk->tail->next = new_tail;
        new_tail->prev = snk->tail;
        snk->tail = new_tail;
    }

    if (!killed(snk))
        paint_map(snk->head, PLAYER);
    else
        snk->alive = false;
}

void gen_apple()
{
    int x, y;
    do {
        x = rand() % CELLS_WIDE;
        y = rand() % CELLS_HIGH;
    } while (map[x][y] != EMPTY);
    map[x][y] = APPLE;
    fill_and_restore(red, (SDL_Rect) {x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT});
}

bool ate_apple(Snake* snk)
{
    int x = snk->head->x;
    int y = snk->head->y;
    return map[x][y] == APPLE;
}

void clear_apple(Snake* snk)
{
    int x = snk->head->x;
    int y = snk->head->y;
    map[x][y] = EMPTY;
}

bool killed(Snake* snk)
{
    int x = snk->head->x;
    int y = snk->head->y;
    return (snk->head->x < 0 || snk->head->x > CELLS_WIDE-1 ||
            snk->head->y < 0 || snk->head->y > CELLS_HIGH-1 ||
            map[x][y] != EMPTY);
}
