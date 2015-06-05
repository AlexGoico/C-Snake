#include "sdl_system.h"

Color wht = { 255, 255, 255, 255 };
Color blk = {   0,   0,   0, 255 };
Color red = { 255,   0,   0, 255 };
Color grn = {   0, 255,   0, 255 };

void sdl_system_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    gWin = SDL_CreateWindow("Test",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWin)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    else
    {
        gRender = SDL_CreateRenderer(gWin, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderClear(gRender);
        set_color(blk);
    }
}

void sdl_system_cleanup()
{
    SDL_DestroyRenderer(gRender);
    SDL_Quit();
}

void set_color(Color c)
{
    last_color = c;
    SDL_SetRenderDrawColor(gRender, c.r, c.g, c.b, c.a);
}

void fill_rect(SDL_Rect rect)
{
    SDL_RenderFillRect(gRender, &rect);
}

void fill_and_restore(Color c, SDL_Rect rect)
{
    Color prev = last_color;
    set_color(c);
    fill_rect(rect);
    set_color(prev);
    last_color = prev;
}

void buffer_flip()
{
    SDL_RenderPresent(gRender);
}
