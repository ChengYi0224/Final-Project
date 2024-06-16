#include "control.h"

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
        return EXIT_FAILURE;
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

        // Titile
        TTF_Font *FontTitle = TTF_OpenFont("assets/fonts/kaiu.ttf", 60);
        DisplayUTF8(renderer, TOML_USE_STRING(mainScript->title), FontTitle, gColorWHITE, &(SDL_Rect){5, 60, 250, 150});

        // Button = Rect{x, y, w, h}, color{r, g, b, a}, isHovered, isClicked
        Button buttonNewGame = {{5, 400, 250, 40}, gColorGREY, 0, 0};
        Button buttonLoadGame = {{5, 450, 250, 40}, gColorGREY, 0, 0};
        Button buttonContinue = {{5, 500, 250, 40}, gColorGREY, 0, 0};
        Button buttonExit = {{5, 550, 250, 40}, gColorGREY, 0, 0};

        // Button Render
        renderButton(renderer, &buttonNewGame);
        renderButton(renderer, &buttonLoadGame);
        renderButton(renderer, &buttonContinue);
        renderButton(renderer, &buttonExit);

        // Button text
        DisplayUTF8(renderer, "New Game", gFontDefault, gColorWHITE, &(SDL_Rect){10, 400, 250, 40});
        DisplayUTF8(renderer, "Load Game", gFontDefault, gColorWHITE, &(SDL_Rect){10, 450, 250, 40});
        DisplayUTF8(renderer, "Continue", gFontDefault, gColorWHITE, &(SDL_Rect){10, 500, 250, 40});
        DisplayUTF8(renderer, "Exit", gFontDefault, gColorWHITE, &(SDL_Rect){10, 550, 250, 40});

        SDL_RenderPresent(renderer);
        // 等待使用者輸入
        SDL_Event event;
        // Button Event Handler
        while (SDL_PollEvent(&event))
        {
            if (handleButton(&event, &buttonNewGame))
            {
                // 處理 New Game 按鈕被點擊*saving的行為
                printf("New game clicked\n");
                return startNewGame(mainScript, saving);
            }
            if (handleButton(&event, &buttonLoadGame))
            {
                // 處理 Load Game 按鈕被點擊的行為
                // 顯示二級menu
                printf("Load game clicked\n");
            }
            if (handleButton(&event, &buttonContinue))
            {
                // 處理 Continue 按鈕被點擊的行為
                printf("Continue clicked\n");
            }
            if (handleButton(&event, &buttonExit))
            {
                // 處理 Exit 按鈕被點擊的行為
                printf("Exit clicked\n");
                return -1;
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
    if (saving->nowScene.event.ok == 1)
    {
        saving->tabCurEvent = toml_table_in(mainScript->event, TOML_USE_STRING(saving->nowScene.event));
    }
    else
    {
        perror("Error setting event");
        return EXIT_FAILURE;
    }
}

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave)
{
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "r")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }
    // 解析存檔
    gRootTabGameSaveRead = toml_parse_file(fpSave, sRead_errmsg, 100);
    if (!gRootTabGameSaveRead)
    {
        printf("Save parsing failed: %s\n", sRead_errmsg);
        free(sRead_errmsg);
        return EXIT_FAILURE;
    }

    // 讀取存檔資料
    GameSave->playerInventory = toml_array_in(gRootTabGameSaveRead, "playerInventory");
    GameSave->nowScene.event = toml_string_in(gRootTabGameSaveRead, "nowScene.event");
    GameSave->nowScene.scene = toml_string_in(gRootTabGameSaveRead, "nowScene.scene");
    GameSave->nowScene.character = toml_string_in(gRootTabGameSaveRead, "nowScene.character");
    GameSave->nowScene.dialogue = toml_string_in(gRootTabGameSaveRead, "nowScene.dialogue");
    // GameSave->nowScene.effect = toml_string_in(gRootTabGameSaveRead, "nowScene.effect");

    // 釋放資源
    fclose(fpSave);
    free(sRead_errmsg);
    atexit(GameSaveTomlTableFree);
    return EXIT_SUCCESS;
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
    // fprintf(fpSave, "effect = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.effect));
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
    (*script).title = toml_string_in(wholeScript, "name");
    (*script).author = toml_string_in(wholeScript, "author");
    (*script).version = toml_string_in(wholeScript, "version");
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

NEXT_ACTION eventHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving)
{
    if (renderer == NULL || script == NULL || saving == NULL)
        return EXIT_FAILURE;

    // 設定scene_t
    scene_t scene = {0};
    scene.scene = toml_string_in(saving->tabCurEvent, "scene");
    scene.dialogue = toml_string_in(saving->tabCurEvent, "dialogue");
    saving->nowScene = scene;
    saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(scene.dialogue));
    // Check if success
    if (saving->tabCurDialogue == NULL)
    {
        perror("Error setting dialogue");
        return _eEMPTY;
    }
    return _eDIALOGUE;
}

NEXT_ACTION dialogueHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving)
{
    // saving setting
    toml_datum_t character = toml_string_in(saving->tabCurDialogue, "character");
    if (character.ok == 1)
    {
        saving->nowScene.character = character;
    }

    SDL_Event event;
    Button nextButton = {.rect = gRectNext, .color = {200, 200, 200, 255}};

    const char *delim = "<br>";
    // text is newly allocated, so that it does not interfere the original text
    char *text = my_strdup(TOML_USE_STRING(toml_string_in(saving->tabCurDialogue, "text")));
    char *token = text;
    char *next = strstr(token, delim);
    while (next != NULL)
    {
        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
        SDL_RenderClear(renderer);
        *next = '\0';
        // 顯示dialogue

        // Set Dialogue Background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &gRectDialogue);
        DisplayUTF8(renderer, token, gFontDefault, gColorWHITE, &gRectText);

        // Next Button
        renderButton(renderer, &nextButton);
        DisplayUTF8(renderer, "Next", gFontDefault, gColorBLACK, &gRectNext);
        SDL_RenderPresent(renderer);
        // 等待Next按鈕被點擊
        int32_t nextClicked = -1;
        while (nextClicked == -1)
        {
            while (SDL_PollEvent(&event))
            {
                if (handleButton(&event, &nextButton) == 1)
                {
                    printf("clicked\n");
                    nextClicked = 1;
                    break;
                }
            }
        }

        token = next + strlen(delim);
        next = strstr(token, delim);
    }

    // 顯示最後一段文字（如果有的話）
    if (*token != '\0')
    {
        SET_DRAW_COLOR(renderer, gColorBackground);
        SDL_RenderClear(renderer);

        // Set Dialogue Background
        SET_DRAW_COLOR(renderer, gColorDialogue);
        // Dialogue
        DisplayUTF8(renderer, token, gFontDefault, gColorWHITE, &gRectText);

        // Button and its text
        renderButton(renderer, &nextButton);
        DisplayUTF8(renderer, "Next", gFontDefault, gColorBLACK, &gRectNext);
        SDL_RenderPresent(renderer);

        // 等待Next按鈕被點擊
        int32_t nextClicked = -1;
        nextClicked = -1;
        while (nextClicked == -1)
        {
            while (SDL_PollEvent(&event))
            {
                if (handleButton(&event, &nextButton) == 1)
                {
                    printf("clicked\n");
                    nextClicked = 1;
                    break;
                }
            }
        }
    }

    // 選擇選項
    toml_array_t *optionArr = toml_array_in(saving->tabCurDialogue, "option");
    if(optionArr != NULL)
    {
        int32_t option_num = toml_array_nelem(optionArr);
        Button optionButtons[option_num];

        for (int32_t i = 0; i < option_num; i++)
        {
            // 讀入當前option的table
            toml_table_t *option = toml_table_at(optionArr, i);
            char *option_text = TOML_USE_STRING(toml_string_in(option, "text"));

            // 設置選項按鈕
            optionButtons[i].rect = gRectOption[i];
            optionButtons[i].color = gColorOptionButton;

            // 顯示選項按鈕
            renderButton(renderer, &optionButtons[i]);
            DisplayUTF8(renderer, option_text, gFontDefault, optionButtons[i].color, &(optionButtons[i].rect));
        }

        SDL_RenderPresent(renderer);

        // 等待選項被點擊
        int32_t choice = -1;
        while (choice == -1)
        {
            while (SDL_PollEvent(&event))
            {
                for (int32_t i = 0; i < option_num; i++)
                {
                    if (handleButton(&event, &optionButtons[i]) == 1)
                    {
                        choice = i;
                        break;
                    }
                }
            }
        }
    free(text);
    return optionHandler(renderer, script, saving, choice); // 根據選項返回下一個動作
    }
    else
    {
        toml_datum_t nextEvent = toml_string_in(saving->tabCurDialogue, "event");
        toml_datum_t nextDialogue = toml_string_in(saving->tabCurDialogue, "next");

        // 判斷接下來要進入event還是next
        if (nextEvent.ok == 1)
        {
            saving->tabCurEvent = toml_table_in(script->event, TOML_USE_STRING(nextEvent));
            return _eEVENT;
        }
        else if (nextDialogue.ok == 1)
        {
            saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(nextDialogue));
            return _eDIALOGUE;
        }
        else
        {
            fprintf(stderr, "%s:%s line %d: No further action informed.\n", __FILE__, __func__, __LINE__);
            fprintf(stderr, "dialogue = %s\n", toml_table_key(saving->tabCurDialogue));
            return _eEMPTY; // Script may be corrupt
        }
    }
}

NEXT_ACTION optionHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving, int32_t optionIdx)
{
    // 參數檢查
    if (renderer == NULL)
        return _eEMPTY;
    if (optionIdx < 0)
        return _eEMPTY;

    // 載入option陣列
    toml_array_t *optionArray = toml_array_in(saving->tabCurDialogue, "option");
    if (optionArray == NULL)
        return _eDIALOGUE;

    // 根據索引查詢玩家所選擇的選項
    toml_table_t *option = toml_table_at(optionArray, optionIdx);
    if (option == NULL)
        return _eEMPTY;

    // 讀取選項內容
    toml_datum_t text = toml_string_in(option, "text");
    DisplayUTF8(renderer, TOML_USE_STRING(text), gFontDefault, gColorLGREY, &gRectDialogue);
    toml_datum_t nextEvent = toml_string_in(option, "event");
    toml_datum_t nextDialogue = toml_string_in(option, "next");

    // 判斷接下來要進入event還是next
    if (nextEvent.ok == 1)
    {
        saving->tabCurEvent = toml_table_in(script->event, TOML_USE_STRING(nextEvent));
        return _eEVENT;
    }
    else if (nextDialogue.ok == 1)
    {
        saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(nextDialogue));
        return _eDIALOGUE;
    }
    else
    {
        fprintf(stderr, "%s:%s line %d: No further action informed.\n", __FILE__, __func__, __LINE__);
        fprintf(stderr, "dialogue = %s, optInx = %d\n", toml_table_key(saving->tabCurDialogue), optionIdx);
        return _eEMPTY; // Script may be corrupt
    }
}

void GameSaveTomlTableFree()
{
    toml_free(gRootTabGameSaveRead);
}