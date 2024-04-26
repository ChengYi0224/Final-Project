#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

int main(int argc, char const *argv[])
{
    printf("%s\n", SDL_GetError());
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}
