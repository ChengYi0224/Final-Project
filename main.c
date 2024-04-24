#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "src/include/toml.h"
#include "src/include/control.h"
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define ScriptPath "ta.toml"

int main(int argc, char const *argv[])
{
    // 打開劇本檔案
    FILE *fpScript = NULL;
    if((fpScript = fopen(ScriptPath, "r")) == NULL){
        perror("Error opening script file");
        return 0;
    }
    
    // SDL系統初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;
    // 建立視窗
    int32_t winW = 1280, winH = 720; // width and height of window
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winW, winH, 0);
    // 建立渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    

    // 執行劇本&遊戲主程式
    scriptRun(fpScript);

    // 程式結束
    fclose(fpScript);
    SDL_Quit();
    return 0;
}
