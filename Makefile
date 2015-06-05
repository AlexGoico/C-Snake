

snake: sdl_system.h sdl_system.c snake.h snake.c
	gcc -std=c11 sdl_system.h sdl_system.c snake.h snake.c -o snake -lSDL2
