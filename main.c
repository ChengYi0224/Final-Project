// #include "src/include/toml.h"
// #include "src/include/control.h"
#include "control.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char const *argv[])
{
    // SDL系統初始化
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init())
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }
    // 建立視窗
    SDL_Window *GameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // 建立渲染器
    //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    // 載入字體
    gFontDefault = TTF_OpenFont("assets/fonts/kaiu.ttf", 30);

    // 圖形介面排版初始化 (建立各個物件需要用的方框)
    // scene顯示方框
    // dialogue顯示方框
    // 文字 ?檢查文字大小、行數
    // 物品
    // 頭像
    // 立繪
    // 當前scene的路徑

    SDL_Event event;
    int32_t game_is_running = 1;
    int32_t ptsize = 40; // 測試用
    TTF_Font *font = TTF_OpenFont("assets/fonts/kaiu.ttf", ptsize); // 測試用                            // 測試用
    Button button = {{300, 250, 200, 100}, {0, 0, 255, 255}, 0, 0};
 
    // 遊戲資料變數
    script_t mainScript = {0};
    scriptRead(ScriptPath, &mainScript);
    
    
    GameSave_t saving = {0};
    NEXT_ACTION NextAction = _eEVENT;

    // 遊戲主迴圈
    while (game_is_running)
    {
        // # 清除畫面
        SDL_RenderClear(renderer);

        // 透過遊戲選單選擇，並根據回傳值執行接下來的劇情
        if(GameStartMenu(renderer, &mainScript, &saving) == -1){
            printf("terminate \n");
            goto end;
        }
        // 遊戲劇情迴圈
        // 退出條件：玩家從選單選擇退出
        while (1)
        {
            SDL_RenderClear(renderer);
            updateInventory(renderer, saving);

            switch (NextAction)
            {
            case _eEVENT:
                // # 下一個事件更新並處理
                NextAction = eventHandler(renderer, &mainScript, &saving);
                break;
            case _eDIALOGUE:
                // # 下一個對話更新並處理
                NextAction = dialogueHandler(renderer, &mainScript, &saving);
                break;
            case _eENDING:
                // # 遊戲結局顯示
                // game_is_running = 0;
                break;
            case _eGAMEQUIT:
                // # 遊戲結束
                game_is_running = 0;
                goto end;
                break;

            case _eEMPTY:
            default:
                // # 錯誤發生
                fprintf(stderr, "Error in Game Script Loop\n");
                goto end;
            }

            // # 音樂播放
            // - 背景音樂:如果有切換則切換，若無則繼續播放
            // - 音效

            // # 畫面繪製

            // 繪製圖像
            // 物品
            // description
            // 背景
            DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.scene, TOML_USE_STRING(saving.nowScene.scene)), "background")), NULL, &gRectBackground); //
            // 立繪
            //DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.character, TOML_USE_STRING(saving.nowScene.character)), "tachie")), NULL, &gRectAvatar);
            // DisplayImg(); // 物品欄
            // 角色頭像
            //DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.character, TOML_USE_STRING(saving.nowScene.character)), "avatar")), NULL, &gRectAvatar);
            // DisplayImg(); // 角色頭像邊框

            // 繪製文字
            // # 聆聽事件(偵測滑鼠/鍵盤輸入) 包含音效  //目前只支援關閉視窗
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    game_is_running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        game_is_running = 0;
                    }
                    break;
                }
            }
            // 點擊選項
            // 物品預覽

            // # 更新畫面
            SDL_RenderPresent(renderer);
        }
    }

// 程式結束，以相反順序釋放資源
end:
    char SavePath[270];
    snprintf(SavePath, sizeof(SavePath), "save/%s", saving.SaveName);
    GameSaveWrite(SavePath, &saving);
    if (mainScript.rootTable)
        toml_free(mainScript.rootTable);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(GameWindow);
    TTF_Quit(); // 關閉TTF
    SDL_Quit(); // 關閉SDL
    return 0;
}
