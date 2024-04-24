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
    // 讀取劇本
    script_t mainScript;
    scriptRead(ScriptPath, &mainScript);

    // SDL系統初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;
    // 建立視窗
    int32_t winW = 1280, winH = 720; // width and height of window
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winW, winH, 0);
    // 建立渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);

    // 遊戲主迴圈
    int32_t frame = 0;
    while (1)
    {
        // 清除畫面
        // SDL_RenderClear(renderer);

        // 更新畫面
        SDL_RenderPresent(renderer);

        // 終止條件
        if(1) break;
    }
    // 程式結束
    SDL_DestroyWindow(GameWindow);
    SDL_DestroyRenderer(renderer);
    SDL_Quit(); // 關閉SDL
    return 0;
}
