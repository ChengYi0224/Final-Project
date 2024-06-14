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
    // 讀取劇本
    script_t mainScript;
    scriptRead(ScriptPath, &mainScript);

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
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);

    // 圖形介面排版初始化 (建立各個物件需要用的方框)
    // scene顯示方框
    SDL_Rect sceneRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    // dialogue顯示方框
    SDL_Rect dialRect = {190, 10 + WINDOW_HEIGHT * 3 / 5, WINDOW_WIDTH - 210, WINDOW_HEIGHT / 3 + 20};
    // 文字 ?檢查文字大小、行數
    SDL_Rect textRect = {dialRect.x + 15, dialRect.y + 3, dialRect.w - 30, dialRect.h - 6};
    // 物品
    //SDL_Rect itemRect = {20, 20, 150, WINDOW_HEIGHT - 40};
    // 頭像
    SDL_Rect faceRect = {WINDOW_WIDTH - 110, 20, 90, WINDOW_HEIGHT - dialRect.h - 60};
    // 立繪
    SDL_Rect standRect = {430, 40, 500, 380};
    // 當前scene的路徑

    SDL_Event event;
    int32_t game_is_running = 1;
    int32_t ptsize = 40; // 測試用
    uint8_t *text = "Aa😂一二三四五六七八九十。😂一二三四五六七八九十。一二三四五六七八九十。一二三四五六七八九十。";
    // char text2[] = "abcdefu rah rah ah ah ah roma roma-ma gaga ooh-la-la ghijk lmnopq"; //測試用
    TTF_Font *font = TTF_OpenFont("assets/fonts/kaiu.ttf", ptsize); // 測試用
    SDL_Color color = {255, 255, 255};                              // 測試用
    Button button = {{300, 250, 200, 100}, {0, 0, 255, 255}, 0, 0};
    char backgroundKey[100] = {0}, text[500] = {0}, characterKey[100] = {0}, *itemKey[2] = {NULL, NULL};


    // 遊戲資料變數
    script_t mainScript = {0};
    GameSave_t saving = {0};
    toml_table_t *currentEvent = NULL;
    toml_table_t *currentDialogue = NULL;

    // 遊戲主迴圈
    while (1)
    {
        // # 清除畫面
        SDL_RenderClear(renderer);

        // 透過遊戲選單選擇，並根據回傳值執行接下來的劇情
        currentEvent = GameStartMenu(renderer, &mainScript, &saving);

        // # 下一個事件更新並處理
        // eventHandler();

        // # 音樂播放
        // - 背景音樂:如果有切換則切換，若無則繼續播放
        // - 音效

        // # 畫面繪製

        // 繪製圖像
        // 物品
        SDL_Rect itemIconRect = {20, 20, 150, WINDOW_HEIGHT - 40}; //icon
        SDL_Rect itemNameRect = {20, 20, 150, WINDOW_HEIGHT - 40}; //name
        SDL_Rect itemDesRect = {20, 20, 150, WINDOW_HEIGHT - 40}; //description
        // 背景
        scene_t cur;
        toml_datum_t itemIcon[2], itemName[2], itemDes[2];
        cur.background = toml_string_in(toml_table_in(script->scene, backgroundKey), "background");
        cur.character = toml_string_in(toml_table_in(script->character, characterKey), "avatar");
        DisplayImg(renderer, cur.background.u.s, NULL, &sceneRect); //
        // DisplayImg(); // 立繪
        // DisplayImg(); // 物品欄
        // DisplayImg(); // 角色頭像
        // DisplayImg(); // 角色頭像邊框

        // 繪製文字
        // textRect.w = WINDOW_WIDTH / 100 * strlen("abcdefu");
        // textRect.h = WINDOW_HEIGHT / 15;
        DisplayImg(renderer, imgtest2, NULL, &dialRect);
        // 物品位置
        for (int32_t i = 0; i < 2; i++)
        {
            if (itemKey[i] != NULL)
            {
                itemIcon[i] = toml_string_in(toml_table_in(script->item, itemKey[i]), icon);
                DisplayImg(renderer, itemIcon[i].u.s, NULL, &itemRect);
                itemRect.y += itemRect.h;
            }
        }

        DisplayImg(renderer, scene.character.u.s, NULL, &faceRect); // 頭像位置
        DisplayImg(renderer, imgtest2, NULL, &standRect);           // 立繪位置
        DisplayUTF8(renderer, text, font, color, &textRect);        // 對話

        // 繪製選項
        // for(size_t i = 0; i < (optionNum); i++){
        //     DisplayButton(); // option
        // }

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
            handleButton(&event, &button);
        }
        renderButton(renderer, &button);
        // 點擊選項
        // 物品預覽

        // # 更新畫面
        SDL_RenderPresent(renderer);

        // # 終止條件
        if (!game_is_running)
        {
            break;
        }
    }

    // 程式結束，以相反順序釋放資源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(GameWindow);
    TTF_Quit(); // 關閉TTF
    SDL_Quit(); // 關閉SDL
    return 0;
}
