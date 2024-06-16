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
    do
    {
        SaveDirEntry = readdir(SaveDir);
        if (SaveDirEntry == NULL)
            break;
    } while (SaveDirEntry->d_name[0] == '.');
    int8_t hasSave = 0;
    if (SaveDirEntry != NULL)
    {
        hasSave = 1;
    }

    while (1)
    {
        // 顯示開始畫面
        SET_DRAW_COLOR(renderer, gColorBLACK);
        SDL_RenderClear(renderer);
        SDL_Rect startRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        if (mainScript->startBackgroundPath.ok == 1)
            DisplayImg(renderer, mainScript->startBackgroundPath.u.s, NULL, &startRect);
        else
            DisplayImg(renderer, StartBackgroundPathDefault, NULL, &startRect);

        // Titile
        TTF_Font *FontTitle = TTF_OpenFont("assets/fonts/kaiu.ttf", 60);
        DisplayUTF8(renderer, TOML_USE_STRING(mainScript->title), FontTitle, gColorBLACK, &(SDL_Rect){5, 60, 250, 150});

        // Authors
        TTF_Font *FontAuthor = TTF_OpenFont("assets/fonts/kaiu.ttf", 20);
        DisplayUTF8(renderer, TOML_USE_STRING(mainScript->author), FontAuthor, gColorBLACK, &(SDL_Rect){25, 130, 400, 50});

        // Button = Rect{x, y, w, h}, color{r, g, b, a}, isHovered, isClicked
        SDL_Color colorMenuButton = gColorDGREY;
        colorMenuButton.a = 215;
        Button buttonNewGame = {{15, 400, 300, 40}, colorMenuButton, 0, 0};
        Button buttonLoadGame = {{15, 450, 300, 40}, colorMenuButton, 0, 0};
        Button buttonContinue = {{15, 500, 300, 80}, colorMenuButton, 0, 0};
        Button buttonExit = {{15, 600, 300, 40}, colorMenuButton, 0, 0};

        // Button Render
        renderButton(renderer, &buttonNewGame);
        renderButton(renderer, &buttonLoadGame);
        renderButton(renderer, &buttonContinue);
        renderButton(renderer, &buttonExit);

        char SaveSelText[260];
        // Button text
        DisplayUTF8(renderer, "New Game", gFontDefault, gColorWHITE, &(SDL_Rect){30, 403, 300, 40});
        if (hasSave)
        {
            DisplayUTF8(renderer, "Choose Save", gFontDefault, gColorWHITE, &(SDL_Rect){30, 453, 300, 40});
            sprintf(SaveSelText, "  > %s", SaveDirEntry->d_name);
            DisplayUTF8(renderer, SaveSelText, gFontDefault, gColorWHITE, &(SDL_Rect){30, 505, 300, 80});
        }
        else
        {
            DisplayUTF8(renderer, "Choose Save", gFontDefault, gColorGREY, &(SDL_Rect){30, 453, 300, 40});
            DisplayUTF8(renderer, "  > (No file)", gFontDefault, gColorGREY, &(SDL_Rect){30, 522, 300, 40});
        }
        DisplayUTF8(renderer, "Exit", gFontDefault, gColorWHITE, &(SDL_Rect){30, 603, 300, 40});

        SDL_RenderPresent(renderer);
        // 等待使用者輸入
        SDL_Event event;
        // Button Event Handler
        while (SDL_PollEvent(&event))
        {
            if (handleButton(&event, &buttonNewGame))
            {
                // 處理 New Game 按鈕被點擊*saving的行為
                // printf("New game clicked\n");
                closedir(SaveDir);
                return startNewGame(mainScript, saving);
            }
            if (hasSave)
            {
                if (handleButton(&event, &buttonLoadGame))
                {
                    // 處理 Choose Save 按鈕被點擊的行為
                    // printf("Choose Save clicked\n");
                    SaveDirEntry = readdir(SaveDir);
                    if (SaveDirEntry == NULL)
                    {
                        rewinddir(SaveDir);
                        do
                        {
                            SaveDirEntry = readdir(SaveDir);
                        } while (SaveDirEntry->d_name[0] == '.');
                    }
                    break;
                }
            }
            if (handleButton(&event, &buttonContinue))
            {
                // 處理 Continue 按鈕被點擊的行為
                // printf("Continue clicked\n");
                printf("opening save at ./Save/%s", SaveDirEntry->d_name);
                if (GameSaveRead(SaveDirEntry->d_name, mainScript, saving))
                {
                    fprintf(stderr, "Error reading save\n");
                    return EXIT_FAILURE;
                }
                closedir(SaveDir);
                return EXIT_SUCCESS;
            }
            if (handleButton(&event, &buttonExit))
            {
                // 處理 Exit 按鈕被點擊的行為
                // printf("Exit clicked\n");
                closedir(SaveDir);
                return -1;
            }
            // check if windows "x" is clicked (close)
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                return _eGAMEQUIT;
            }
        }
    }
}

// Set the first event "start"
int8_t startNewGame(script_t *mainScript, GameSave_t *saving)
{
    scene_t sceneStart;
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
    toml_table_t *player = toml_table_in(mainScript->rootTable, "player");
    if (player != NULL)
    {
        sceneStart.event = toml_string_in(player, "starter");
        // Inventory
        toml_array_t *playerInventory = toml_array_in(player, "inventory");
        if (playerInventory != NULL)
        {
            saving->nPlayerItem = toml_array_nelem(playerInventory);
            (saving->playerItem) = calloc(saving->nPlayerItem, sizeof(toml_table_t *));
            for (int32_t i = 0; i < saving->nPlayerItem; i++)
            {
                toml_datum_t item = toml_string_at(playerInventory, i);
                saving->playerItem[i] = toml_table_in(mainScript->item, TOML_USE_STRING(item));
            }
        }
    }
    // 設定scene
    //sceneStart.event.u.s = "start";
    //sceneStart.event.ok = 1;
    sceneStart.scene = toml_string_in(eventStart, "scene");
    sceneStart.character = toml_string_in(eventStart, "character");
    sceneStart.dialogue = toml_string_in(eventStart, "dialogue");
    sceneStart.effect = toml_string_in(eventStart, "effect");
    saving->nowScene = sceneStart;

    snprintf(saving->SaveName, sizeof(saving->SaveName), "%s", asctime(getLocalTime()));

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

int8_t GameSaveRead(char *SaveName, script_t *mainScript, GameSave_t *saving)
{
    // 打開劇本檔案
    char SavePath[256];
    snprintf(SavePath, sizeof(SavePath), "./save/%s", SaveName);
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "r")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }
    strncpy(saving->SaveName, SaveName, sizeof(saving->SaveName));
    // 解析存檔
    gRootTabGameSaveRead = toml_parse_file(fpSave, sRead_errmsg, 100);
    if (!gRootTabGameSaveRead)
    {
        printf("Save parsing failed: %s\n", sRead_errmsg);
        free(sRead_errmsg);
        return EXIT_FAILURE;
    }

    // 讀取存檔資料
    toml_array_t * playerInventory = toml_array_in(gRootTabGameSaveRead, "playerInventory");
    if (playerInventory != NULL)
    {
        saving->nPlayerItem = toml_array_nelem(playerInventory);
        (saving->playerItem) = calloc(saving->nPlayerItem, sizeof(toml_table_t *));
        for(int32_t i = 0; i < saving->nPlayerItem; i++){
            toml_datum_t item = toml_string_at(playerInventory, i);
            saving->playerItem[i] = toml_table_in(mainScript->item, TOML_USE_STRING(item));
        }
    }
    toml_table_t *SaveNowScene = toml_table_in(gRootTabGameSaveRead, "nowScene");
    toml_datum_t valEvent = toml_string_in(SaveNowScene, "event");
    toml_datum_t valScene = toml_string_in(SaveNowScene, "scene");
    toml_datum_t valCharacter = toml_string_in(SaveNowScene, "character");
    toml_datum_t valDialogue = toml_string_in(SaveNowScene, "dialogue");
    // toml_datum_t valEffect = toml_string_in(gRootTabGameSaveRead, "nowScene.effect");

    // 把GameSave連結到mainScript的rootTable
    saving->tabCurEvent = toml_table_in(mainScript->event, TOML_USE_STRING(valEvent));
    saving->tabCurDialogue = toml_table_in(mainScript->dialogue, TOML_USE_STRING(valDialogue));
    toml_set_string(&saving->nowScene.event, TOML_USE_STRING(valEvent));
    toml_set_string(&saving->nowScene.scene, TOML_USE_STRING(valScene));
    toml_set_string(&saving->nowScene.character, TOML_USE_STRING(valCharacter));
    toml_set_string(&saving->nowScene.dialogue, TOML_USE_STRING(valDialogue));
    // toml_set_string(&GameSave->nowScene.effect, TOML_USE_STRING(valEffect));

    // 設定saving->tabCur
    saving->tabCurEvent = toml_table_in(mainScript->event, TOML_USE_STRING(valEvent));
    saving->tabCurDialogue = toml_table_in(mainScript->dialogue, TOML_USE_STRING(valDialogue));

    // 釋放資源
    fclose(fpSave);
    free(sRead_errmsg);
    GameSaveTomlTableFree();
    return EXIT_SUCCESS;
}

int8_t GameSaveWrite(char *SaveName, GameSave_t *GameSave)
{
    char SavePath[256];
    snprintf(SavePath, sizeof(SavePath), "./save/%s", SaveName);
    // 打開劇本檔案
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "w")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }

    // 開始進行存檔，使用toml格式

    // 玩家背包
    if (*GameSave->playerItem != NULL)
    {
        fprintf(fpSave, "playerInventory = [\n");
        int32_t sizeInv = GameSave->nPlayerItem;
        for (int i = 0; i < sizeInv; i++)
        {
            fprintf(fpSave, "\"%s\",\n", toml_table_key(GameSave->playerItem[i]));
        }
        fprintf(fpSave, "]\n");
    }

    // 當前場景紀錄
    fprintf(fpSave, "[nowScene]\n");
    fprintf(fpSave, "event = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.event));
    fprintf(fpSave, "scene = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.scene));
    fprintf(fpSave, "character = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.character));
    fprintf(fpSave, "dialogue = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.dialogue));
    // fprintf(fpSave, "effect = \"%s\"\n", TOML_USE_STRING(GameSave->nowScene.effect));
    fprintf(fpSave, "\n");

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
    (*script).license = toml_string_in(wholeScript, "license");
    (*script).startBackgroundPath = toml_string_in(wholeScript, "startBackgroundPath");
    
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

    // 設定
    // saving->nowScene.event.u.s = toml_table_key(saving->tabCurEvent);
    // saving->nowScene.event.ok = 1;
    saving->nowScene.scene = toml_string_in(saving->tabCurEvent, "scene");
    saving->nowScene.dialogue = toml_string_in(saving->tabCurEvent, "dialogue");
    saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(saving->nowScene.dialogue));
    // Check if success
    if (saving->tabCurDialogue == NULL)
    {
        fprintf(stderr,"Error setting dialogue\n");
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
    Button nextButton = {.rect = gRectNext, .color = gColorNextButton, .isHovered = 0, .isClicked = 0};

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

        // Set Scene
        DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(script->scene, TOML_USE_STRING(saving->nowScene.scene)), "background")), NULL, &gRectBackground);
        /// Renderer Tachie
        renderTachie(renderer, script, saving);

        // Set Dialogue
        SET_DRAW_COLOR(renderer, gColorDialogue); // Background Color
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &gRectDialogue); // Background
        DisplayUTF8(renderer, token, gFontDefault, gColorWHITE, &gRectText);

        // Render Avatar
        renderAvatar(renderer, script, saving);

        // Next Button
        renderButton(renderer, &nextButton);
        DisplayUTF8(renderer, "  NEXT", gFontDefault, gColorNextText, &gRectNext);

        SDL_RenderPresent(renderer);
        // 等待Next按鈕被點擊
        int32_t nextClicked = -1;
        while (nextClicked == -1)
        {
            while (SDL_PollEvent(&event))
            {
                if (handleButton(&event, &nextButton) == 1)
                {
                    // printf("clicked\n");
                    nextClicked = 1;
                    break;
                }
                // check if windows "x" is clicked (close)
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    return _eGAMEQUIT;
                }
            }
        }

        token = next + strlen(delim);
        next = strstr(token, delim);
    }

    // 顯示最後一段文字（如果有的話）
    if (*token != '\0')
    {
        // SET_DRAW_COLOR(renderer, gColorBackground);
        SDL_RenderClear(renderer);
        DisplayImg(renderer, TOML_USE_STRING(toml_string_in(toml_table_in(script->scene, TOML_USE_STRING(saving->nowScene.scene)), "background")), NULL, &gRectBackground);
        // Renderer Tachie 
        renderTachie(renderer, script, saving);
        // Set Dialogue Background
        SET_DRAW_COLOR(renderer, gColorDialogue);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &gRectDialogue);
        // Dialogue
        DisplayUTF8(renderer, token, gFontDefault, gColorWHITE, &gRectText);

        // Render Avatar
        renderAvatar(renderer, script, saving);

        // Button and its text
        renderButton(renderer, &nextButton);
        DisplayUTF8(renderer, "  NEXT", gFontDefault, gColorNextText, &gRectNext);

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
                    // printf("clicked\n");
                    nextClicked = 1;
                    break;
                }
                // check if windows "x" is clicked (close)
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    return _eGAMEQUIT;
                }
            }
        }
    }

    // 選擇選項
    toml_array_t *optionArr = toml_array_in(saving->tabCurDialogue, "option");
    if (optionArr != NULL)
    {
        int32_t option_num = toml_array_nelem(optionArr);
        Button optionButtons[option_num];
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (int32_t i = 0; i < option_num; i++)
        {
            // 讀入當前option的table
            toml_table_t *option = toml_table_at(optionArr, i);
            char *option_text = TOML_USE_STRING(toml_string_in(option, "text"));

            // 設置選項按鈕
            optionButtons[i].rect = gRectOption[i];
            optionButtons[i].color = gColorOptionButton;
            optionButtons[i].isHovered = 0;
            optionButtons[i].isClicked = 0;

            // 顯示選項按鈕
            // printf("opt %d button isHovered = %u\n", i, optionButtons[i].isHovered);
            renderButton(renderer, &optionButtons[i]);
            DisplayUTF8(renderer, option_text, gFontDefault, gColorOptionText, &(optionButtons[i].rect));
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
                // check if windows "x" is clicked (close)
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    return _eGAMEQUIT;
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
    DisplayUTF8(renderer, TOML_USE_STRING(text), gFontDefault, gColorLGREY, &gRectOption[0]);
    toml_datum_t nextEvent = toml_string_in(option, "event");
    toml_datum_t nextDialogue = toml_string_in(option, "next");

    // 判斷接下來要進入event還是next
    if (nextEvent.ok == 1)
    {
        saving->tabCurEvent = toml_table_in(script->event, TOML_USE_STRING(nextEvent));
        saving->nowScene.event = nextEvent;
        return _eEVENT;
    }
    else if (nextDialogue.ok == 1)
    {
        saving->tabCurDialogue = toml_table_in(script->dialogue, TOML_USE_STRING(nextDialogue));
        saving->nowScene.dialogue = nextDialogue;
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