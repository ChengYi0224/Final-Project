#include "src/include/toml.h"
#include "src/include/control.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define imgtest "assets/scenes/2K.jpg"
#define imgtest2 "assets/scenes/aut.jpg"
#define ScriptPath "ta.toml"

int main(int argc, char const *argv[])
{
    // 讀取劇本
    script_t mainScript;
    scriptRead(ScriptPath, &mainScript);
    printf("Debug msg 1\n");

    // SDL系統初始化
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL_Init failed: %s\n", SDL_GetError() );
        return 1;
    }
    printf("Debug msg 2\n");
    if (TTF_Init()){
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }
    printf("Debug msg 3\n");
    // 建立視窗
    int32_t winW = 1280, winH = 720; // width and height of window
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winW, winH, 0);
    // 建立渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, 0);
    printf("Debug msg 4\n");

    // 圖形介面排版初始化 (建立各個物件需要用的方框)
    // scene顯示方框
    SDL_Rect sceneRect = {0, 0, winW, winH};
    // dialogue顯示方框
    SDL_Rect dialRect = {30, 10 + winH * 2 / 3, winW - 60, winH / 3 - 30};
    // 文字 ?檢查文字大小、行數
    SDL_Rect textRect = {dialRect.x + 15, dialRect.y + 3, dialRect.w - 30, dialRect.h - 6};
    // 當前scene的路徑

    SDL_Event event;
    int32_t game_is_running = 1;
    int32_t ptsize = 40; //測試用
    //wchar_t ctext[] = L"你好";
    char text[] = "abcdefu rah rah ah ah ah roma roma-ma gaga ooh-la-la ghijk lmnopq"; //測試用
    TTF_Font * font = TTF_OpenFont( "assets/fonts/Amiri-Bold.ttf" , ptsize); //測試用
    SDL_Color color = {255, 255, 255}; //測試用
    // 遊戲主迴圈
    while (1)
    {
        // # 清除畫面
        SDL_RenderClear(renderer);

        // # 下一個事件更新並處理
        // eventHandler();

        // # 音樂播放
        // - 背景音樂:如果有切換則切換，若無則繼續播放
        // - 音效

        // # 畫面繪製

        // 繪製圖像
        // 背景
        DisplayImg(renderer, imgtest, NULL, &sceneRect);//
        // DisplayImg(); // 立繪
        // DisplayImg(); // 物品欄
        // DisplayImg(); // 角色頭像
        // DisplayImg(); // 角色頭像邊框

        // 繪製文字
        //textRect.w = winW / 100 * strlen("abcdefu");
        //textRect.h = winH / 15;
        DisplayImg(renderer, imgtest2, NULL, &dialRect);
        DisplayText(renderer, text, font, color, &textRect); // 對話

        // 繪製選項
        // for(size_t i = 0; i < (optionNum); i++){
        //     DisplayButton(); // option
        // }

        // # 聆聽事件(偵測滑鼠/鍵盤輸入) 包含音效  //目前只支援關閉視窗
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game_is_running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        game_is_running = 0;
                    }
                    break;
            }
        }
        // 點擊選項
        // 物品預覽

        // # 更新畫面
        SDL_RenderPresent(renderer);

        // # 終止條件
        if(!game_is_running) break;
    }

    // 程式結束，以相反順序釋放資源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(GameWindow);
    TTF_Quit(); // 關閉TTF
    SDL_Quit(); // 關閉SDL
    return 0;
}
