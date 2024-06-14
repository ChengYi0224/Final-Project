#include "../include/toml.h"
#include "../include/control.h"

// The very entry of the program
int8_t GameStartMenu(SDL_Renderer *renderer, script_t *mainScript, GameSave_t *saving)
{
    // 檢查遊戲存檔資料夾狀態
    // 如果不存在，則創建資料夾
    struct stat st = {0};
    if (stat("./save", &st) == -1)
    {
        mkdir("./save", 0700);
    }

    // 開啟存檔資料夾
    DIR *SaveDir;
    struct dirent *SaveDirEntry;
    SaveDir = opendir("./save");
    if (SaveDir == NULL)
    {
        perror("Error opening save directory");
        return NULL;
    }

    // 檢查是否存在任何存檔
    SaveDirEntry = readdir(SaveDir);
    closedir(SaveDir);

    while (1)
    {
        // 顯示開始畫面
        SDL_Rect startRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        if (mainScript->startBackgroundPath.ok == 1)
            DisplayImg(renderer, mainScript->startBackgroundPath.u.s, NULL, &startRect);
        else
            DisplayImg(renderer, StartBackgroundPathDefault, NULL, &startRect);

        // Button = Rect{x, y, w, h}, color{r, g, b, a}, isHovered, isClicked
        Button buttonNewGame = {{5, 400, 50, 20}, {200, 200, 200, 255}, 0, 0};
        Button buttonLoadGame = {{5, 450, 50, 20}, {200, 200, 200, 255}, 0, 0};
        Button buttonContinue = {{5, 500, 50, 20}, {200, 200, 200, 255}, 0, 0};
        Button buttonVolume = {{5, 550, 50, 20}, {200, 200, 200, 255}, 0, 0};
        Button buttonExit = {{5, 600, 50, 20}, {200, 200, 200, 255}, 0, 0};

        // Button Render
        renderButton(renderer, &buttonNewGame);
        renderButton(renderer, &buttonLoadGame);
        renderButton(renderer, &buttonContinue);
        renderButton(renderer, &buttonVolume);
        renderButton(renderer, &buttonExit);
        SDL_RenderPresent(renderer);

        // 等待使用者輸入
        SDL_Event event;
        // Button Event Handler
        while (SDL_PollEvent(&event))
        {
            if (handleButton(&event, &buttonNewGame))
            {
                // 處理 New Game 按鈕被點擊*saving的行為
                return startNewGame(mainScript, saving);
            }
            if (handleButton(&event, &buttonLoadGame))
            {
                // 處理 Load Game 按鈕被點擊的行為
                // 顯示二級menu
            }
            if (handleButton(&event, &buttonContinue))
            {
                // 處理 Continue 按鈕被點擊的行為
            }
            if (handleButton(&event, &buttonVolume))
            {
                // 處理 Volume 按鈕被點擊的行為
            }
            if (handleButton(&event, &buttonExit))
            {
                // 處理 Exit 按鈕被點擊的行為
                return NULL;
            }
        }
        /*
        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return NULL;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    break;
                }
            }
        }
        break;
        */
    }
}

// Set the first event "start"
int8_t startNewGame(script_t *mainScript, GameSave_t *saving)
{
    if (scriptRead(ScriptPath, mainScript))
    {
        perror("Error reading script");
        return EXIT_FAILURE;
    }
    toml_table_t *eventStart = toml_table_in(mainScript->event, "start");
    if (eventStart == NULL)
    {
        perror("Error reading start event");
        return EXIT_FAILURE;
    }
    // 設定scene_t
    scene_t sceneStart;
    sceneStart.scene = toml_string_in(eventStart, "scene");
    sceneStart.character = toml_string_in(eventStart, "character");
    sceneStart.dialogue = toml_string_in(eventStart, "dialogue");
    sceneStart.effect = toml_string_in(eventStart, "effect");
    saving->nowScene = sceneStart;

    // 設定event
    toml_set_string(&(saving->nowScene.event), "start");
    if(saving->nowScene.event.ok == 1){
        saving->tabCurEvent = toml_table_in(mainScript->event, TOML_USE_STRING(saving->nowScene.event));
    }else{
        perror("Error setting event");
        return EXIT_FAILURE;
    }
}

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave)
{
}

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave)
{
    // 打開劇本檔案
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "w")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }

    // 開始進行存檔，使用toml格式

    // 玩家背包
    fprintf(fpSave, "playerInventory = [\n");
    int32_t sizeInv = toml_array_nelem(GameSave->playerInventory);
    for (int i = 0; i < sizeInv; i++)
    {
        fprintf(fpSave, "\"%s\",\n", TOML_USE_STRING(toml_string_at(GameSave->playerInventory, i)));
    }
    fprintf(fpSave, "]\n");


    // 當前場景紀錄
    fprintf(fpSave, "nowScene = {\n");
    fprintf(fpSave, "event = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.event));
    fprintf(fpSave, "scene = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.scene));
    fprintf(fpSave, "character = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.character));
    fprintf(fpSave, "dialogue = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.dialogue));
    //fprintf(fpSave, "effect = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.effect));
    fprintf(fpSave, "}\n");

    fclose(fpSave);
    return EXIT_SUCCESS;
}

//  return 0 when succeeded
//  return 1 when failed
int8_t scriptRead(char *scriptPath, script_t *script)
{
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpScript = NULL;
    if ((fpScript = fopen(scriptPath, "r")) == NULL)
    {
        perror("Error opening script file");
        return EXIT_FAILURE;
    }
    // 解析劇本
    toml_table_t *wholeScript = toml_parse_file(fpScript, sRead_errmsg, 100);
    if (!wholeScript)
    {
        printf("Script parsing failed: %s\n", sRead_errmsg);
        free(sRead_errmsg);
        return EXIT_FAILURE;
    }
    // 讀取標頭資料
    (*script).description = toml_string_in(wholeScript, "description");

    // 分類物件
    (*script).item = toml_table_in(wholeScript, "item");
    (*script).event = toml_table_in(wholeScript, "event");
    (*script).scene = toml_table_in(wholeScript, "scene");
    (*script).dialogue = toml_table_in(wholeScript, "dialogue");
    (*script).character = toml_table_in(wholeScript, "character");

    script->rootTable = wholeScript;

    // 釋放資源
    fclose(fpScript);
    free(sRead_errmsg);
    return EXIT_SUCCESS;
}

int8_t eventHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving)
{
    if (renderer == NULL || script == NULL || saving == NULL)
        return EXIT_FAILURE;
    
    // 設定scene_t
    scene_t scene = {0};
    scene.scene = toml_string_in(saving->tabCurEvent, "scene");
    scene.dialogue = toml_string_in(saving->tabCurEvent, "dialogue");
    saving->nowScene = scene;
    saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(scene.dialogue));


}

<<<<<<< Updated upstream
toml_table_t *dialogueHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving, toml_table_t *dialogue)
{
    SDL_Event event;
=======
int8_t dialogueHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving){
    // scene顯示方框
    SDL_Rect sceneRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    // dialogue顯示方框
>>>>>>> Stashed changes
    SDL_Rect dialRect = {190, 10 + WINDOW_HEIGHT * 3 / 5, WINDOW_WIDTH - 210, WINDOW_HEIGHT / 3 + 20};
    SDL_Rect textRect = {dialRect.x + 15, dialRect.y + 3, dialRect.w - 30, dialRect.h - 6};
    int32_t ptsize = 40; // 測試用
    TTF_Font *font = TTF_OpenFont("assets/fonts/kaiu.ttf", ptsize); // 測試用
    SDL_Color color = {255, 255, 255};
    Button nextButton, optionButton;
    char delim[] = "<br>";
    char *text = TOML_USE_STRING(toml_string_in(dialogue, "text"));
    char *token = strtok(text, delim);
    while (token != NULL)
    {
        DisplayUTF8(renderer, token, font, color, &textRect);
        renderButton(renderer, &nextButton);
        SDL_RenderPresent(renderer);
        //按Next按鈕
        while (SDL_PollEvent(&event))
        {
            if(handleButton(&event, &nextButton) == 1) break;
        }
        token = strtok(NULL, delim);
    }
    //選擇選項
    int option_num = toml_array_length(toml_array_in(dialogue, "options"));
    for (int i = 0; i < option_num; i++)
    {
        toml_table_t *option = toml_table_at(toml_array_in(dialogue, "options"), i);
        char *option_text = TOML_USE_STRING(toml_string_in(option, "text"));
        //顯示選項
        DisplayUTF8(renderer, option_text, font, color, &textRect);
        renderButton(renderer, &optionButton);
        //按下選項
        while (SDL_PollEvent(&event))
        {
            if(handleButton(&event, &button) == 1) break;
        }
            //儲存選擇
            saving->choice = i;
            return dialogue;
    }
}