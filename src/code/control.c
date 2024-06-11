/*
    程式包含以下功能:
    -   滑鼠 / 鍵盤輸入(取消，移至主程式或是其他function實現)
    -   介面顯示(取消，移至主程式或是其他function實現)
        -   台詞顯示
        -   背景顯示
        -   頭像顯示
        -   特效
    -   從script.toml讀取劇本
        1.  讀取劇本
        2.  分類物件
        3.  依序執行(取消，移至主程式或是其他function實現)
*/
#include "../include/control.h"

//  return 0 when succeeded
//  return 1 when failed
int8_t scriptRead(char *scriptPath, script_t *script)
{
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE * fpScript = NULL;
    if ((fpScript = fopen(scriptPath, "r")) == NULL)
    {
        perror("Error opening script file");
        return EXIT_FAILURE;
    }
    // 解析劇本
    toml_table_t *wholeScript   = toml_parse_file(fpScript, sRead_errmsg, 100);
    if(!wholeScript){
        printf("Script parsing failed: %s\n", sRead_errmsg);
        free(sRead_errmsg);
        return EXIT_FAILURE;
    }
    // 讀取標頭資料
    (*script).title     = toml_string_in(wholeScript, "name");
    (*script).author    = toml_string_in(wholeScript, "author");
    (*script).version   = toml_string_in(wholeScript, "version");
    (*script).license   = toml_string_in(wholeScript, "license");
    (*script).description = toml_string_in(wholeScript, "description");

    // 分類物件
    (*script).item      = toml_table_in(wholeScript, "item");
    (*script).event     = toml_table_in(wholeScript, "event");
    (*script).scene     = toml_table_in(wholeScript, "scene");
    (*script).dialogue  = toml_table_in(wholeScript, "dialogue");
    (*script).character = toml_table_in(wholeScript, "character");

    // 釋放資源
    toml_free(wholeScript);
    fclose(fpScript);
    free(sRead_errmsg);
    return EXIT_SUCCESS;
}

// read the script to configure the start menu
// otherwise, remain default
// return a table of the first event
toml_table_t *GameStartMenu(SDL_Renderer *renderer, script_t mainScript){
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
    if(SaveDirEntry == NULL)
    {
        while(1){
            // 顯示開始畫面
            SDL_Rect startRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            if(mainScript.startBackgroundPath.ok == 1)
                DisplayImg(renderer, mainScript.startBackgroundPath.u.s, NULL, &startRect);
            else
            {
                
            }
            
            SDL_RenderPresent(renderer);
            // 等待使用者輸入
            SDL_Event event;
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
        }
    }


}

int8_t DisplayImg(SDL_Renderer *renderer, char *imgPath, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
    // 讀取圖片
    SDL_Surface *image = IMG_Load(imgPath);
    // 建立材質
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    // 顯示
    SDL_RenderCopy(renderer, texture, srcRect, dstRect);
    // 釋放資源
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

int8_t DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect)
{
    // 建立材質
    //SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Surface *textSurface = TTF_RenderUnicode_Blended_Wrapped(font, text, color, dstRect->w);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (texture == NULL)
    {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    dstRect->w = textSurface->w;
    if (dstRect->h > textSurface->h) dstRect->h = textSurface->h;
    SDL_RenderCopy(renderer, texture, NULL, dstRect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);

    return 1;
}

int8_t DisplayUTF8(SDL_Renderer *renderer, uint8_t *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect)
{
    // 建立材質
    //SDL_Surface *textSurface = TTF_RenderUTF8_Solid_Wrapped(font, text, color, dstRect->w);
    //SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, dstRect->w);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (texture == NULL)
    {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    dstRect->w = textSurface->w;
    if (dstRect->h > textSurface->h) dstRect->h = textSurface->h;
    SDL_RenderCopy(renderer, texture, NULL, dstRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);

    return 1;
}

int8_t DisplayItem(SDL_Renderer *renderer, toml_table_t *item, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
    //SDL_Texture *texture = LoadTexture(renderer, item->string["image"]);
}

int8_t renderButton(SDL_Renderer *renderer, Button *button)
{
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    if (button->isHovered) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);  // Highlight color
        SDL_RenderFillRect(renderer, &button->rect);
    }

    if (button->isClicked) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);  // Clicked color
        SDL_RenderFillRect(renderer, &button->rect);
    }
}

<<<<<<< Updated upstream

int8_t handleButton(SDL_Event *event, Button *button)
{
=======
void handleButton(SDL_Event *event, Button *button) {
>>>>>>> Stashed changes
    switch (event->type) {
        case SDL_MOUSEMOTION:
            button->isHovered = SDL_PointInRect(&(SDL_Point){event->motion.x, event->motion.y}, &button->rect);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT && button->isHovered) {
                button->isClicked = 1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT) {
                if (button->isClicked && button->isHovered) {
                    // Button action triggered here
                    //SDL_Log("Button clicked!");
                }
                button->isClicked = 0;
            }
            break;
    }
}

<<<<<<< Updated upstream
int8_t optionHandler(SDL_Renderer *renderer, script_t script, toml_table_t *event, int32_t optionIdx)
{
    if(renderer == NULL)
    {
        return EXIT_FAILURE;
    }
    if(event == NULL)
    {
        return EXIT_FAILURE;
    }
    if(optionIdx < 0)
    {
        return EXIT_FAILURE;
    }
    toml_array_t *optionArray = toml_array_in(event, "options");
    if(optionArray == NULL)
    {
        return EXIT_FAILURE;
    }
}
=======


>>>>>>> Stashed changes
