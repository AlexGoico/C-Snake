#ifndef SDL_SYSTEM_H_
#define SDL_SYSTEM_H_

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define CELL_WIDTH 20
#define CELL_HEIGHT 20

// Grid units.
#define CELLS_WIDE (SCREEN_WIDTH / CELL_WIDTH)
#define CELLS_HIGH (SCREEN_HEIGHT / CELL_HEIGHT)

SDL_Window* gWin;
SDL_Renderer* gRender;

typedef struct { int r, g, b, a; } Color;

Color last_color;

void sdl_system_init();
void sdl_system_cleanup();
void set_color(Color c);
void fill_rect(SDL_Rect rect);
void fill_and_restore(Color c, SDL_Rect rect);

void buffer_flip();

#endif
