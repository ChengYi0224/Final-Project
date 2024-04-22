#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "src/include/toml.h"
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ScriptPath "ta.toml"

int main(int argc, char const *argv[])
{
    // 打開劇本檔案
    FILE *fpScript = NULL;
    if(fpScript = fopen(ScriptPath, "r") == NULL){
        perror("Error opening script file");
        return 0;
    }

    // SDL系統初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;
    // 建立視窗
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    // 建立渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    // 讀取圖片
    SDL_Surface *image = IMG_Load("assets/images/test.png");
    // 建立材質
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);


    // 程式結束
    fclose(fpScript);
    SDL_Quit();
    return 0;
}
