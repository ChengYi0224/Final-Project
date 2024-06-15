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
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Renderer *renderer = SDL_CreateRenderer(GameWindow, -1, SDL_RENDERER_ACCELERATED);
    // 載入字體
    gFontDefault = TTF_OpenFont("assets/fonts/kaiu.ttf", 40);

    // 圖形介面排版初始化 (建立各個物件需要用的方框)
    // scene顯示方框
    // dialogue顯示方框
    // 文字 ?檢查文字大小、行數
    // 物品
    // 頭像
    SDL_Rect faceRect = {WINDOW_WIDTH - 110, 20, 90, WINDOW_HEIGHT - dialRect.h - 60};
    // 立繪
    // 當前scene的路徑

    SDL_Event event;
    int32_t game_is_running = 1;
    int32_t ptsize = 40; // 測試用
    uint8_t *textTest = "Aa😂一二三四五六七八九十。😂一二三四五六七八九十。一二三四五六七八九十。一二三四五六七八九十。";
    // char text2[] = "abcdefu rah rah ah ah ah roma roma-ma gaga ooh-la-la ghijk lmnopq"; //測試用
    TTF_Font *font = TTF_OpenFont("assets/fonts/kaiu.ttf", ptsize); // 測試用
    SDL_Color color = {255, 255, 255};                              // 測試用
    Button button = {{300, 250, 200, 100}, {0, 0, 255, 255}, 0, 0};
    char backgroundKey[100] = {0}, text[500] = {0}, characterKey[100] = {0}, *itemKey[2] = {NULL, NULL};

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
        if(GameStartMenu(renderer, &mainScript, &saving) == 0)
            goto end;

        // 遊戲劇情迴圈
        // 退出條件：玩家從選單選擇退出
        while (1)
        {
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
                // # 遊戲結束
                // game_is_running = 0;
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
            SDL_Rect itemIconRect = {20, 20, 150, WINDOW_HEIGHT - 40}; // icon
            SDL_Rect itemNameRect = {20, 20, 150, WINDOW_HEIGHT - 40}; // name
            SDL_Rect itemDesRect = {20, 20, 150, WINDOW_HEIGHT - 40};  // description
            // 背景
            toml_datum_t itemIcon[2], itemName[2], itemDes[2];
            // cur.scene = toml_string_in(toml_table_in(mainScript.scene, backgroundKey), "background");
            //cur.character = toml_string_in(toml_table_in(mainScript.character, characterKey), "avatar");
            DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.scene, TOML_USE_STRING(saving.nowscene.scene)), "background")), NULL, &gRectBackground); //
            // DisplayImg(); // 立繪
            DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.character, TOML_USE_STRING(saving.nowscene.character)), "tachie")), NULL, &gRectAvatar);
            // DisplayImg(); // 物品欄
            // DisplayImg(); // 角色頭像
            DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(mainScript.character, TOML_USE_STRING(saving.nowscene.character)), "avatar")), NULL, &gRectAvatar);
            // DisplayImg(); // 角色頭像邊框

            // 繪製文字
            // textRect.w = WINDOW_WIDTH / 100 * strlen("abcdefu");
            // textRect.h = WINDOW_HEIGHT / 15;
            // DisplayImg(renderer, imgtest2, NULL, &dialRect);
            // 物品位置
            /*
            for (int32_t i = 0; i < 2; i++)
            {
                if (itemKey[i] != NULL)
                {
                    itemIcon[i] = toml_string_in(toml_table_in(script.item, itemKey[i]), icon);
                    DisplayImg(renderer, itemIcon[i].u.s, NULL, &itemRect);
                    itemRect.y += itemRect.h;
                }
            }
            */
            // DisplayImg(renderer, scene.character.u.s, NULL, &faceRect); // 頭像位置

            DisplayUTF8(renderer, textTest, font, color, &textRect); // 對話

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
        }
    }

// 程式結束，以相反順序釋放資源
end:
    if (mainScript.rootTable)
        toml_free(mainScript.rootTable);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(GameWindow);
    TTF_Quit(); // 關閉TTF
    SDL_Quit(); // 關閉SDL
    return 0;
}
