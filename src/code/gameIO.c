#include "../include/toml.h"
#include "../include/control.h"

// read the script to configure the start menu
// otherwise, remain default
// return a table of the first event
toml_table_t *GameStartMenu(SDL_Renderer *renderer, script_t mainScript)
{
    struct stat st = {0};

    if (stat("./save", &st) == -1)
    {
        mkdir("./save", 0700);
    }

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

    while (1)
    {
        // 顯示開始畫面
        SDL_Rect startRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        if (mainScript.startBackgroundPath.ok == 1)
            DisplayImg(renderer, mainScript.startBackgroundPath.u.s, NULL, &startRect);
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
                // 處理 New Game 按鈕被點擊的行為
            }
            if (handleButton(&event, &buttonLoadGame))
            {
                // 處理 Load Game 按鈕被點擊的行為
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

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave)
{

}

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave)
{
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "w")) == NULL)
    {
        perror("Error opening save");
        return EXIT_FAILURE;
    }

    // 開始進行存檔，使用toml格式
    fprintf(fpSave,"sizePlayerInventory = %ld\n", GameSave->sizePlayerInventory.u.i);
    fprintf(fpSave, "[playerInventory]\n");
    for (int i = 0; i < GameSave->sizePlayerInventory.u.i; i++)
    {
        fprintf(fpSave, "item%d = %s\n", i, GameSave->playerInventory);
    }
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

    // 釋放資源
    toml_free(wholeScript);
    fclose(fpScript);
    free(sRead_errmsg);
    return EXIT_SUCCESS;
}