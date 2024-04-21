#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "src/include/toml.h"
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char const *argv[])
{
    // setting initial state
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    return 0;
}
