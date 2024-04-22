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
    
    // create window
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    // load image
    SDL_Surface *image = IMG_Load("assets/images/test.png");
    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    // create texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

    return 0;
}
