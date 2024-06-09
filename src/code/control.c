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
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
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
        return 0;
    }
    // 解析劇本
    toml_table_t *wholeScript   = toml_parse_file(fpScript, sRead_errmsg, 100);
    if(!wholeScript){
        printf("Script parsing failed: %s\n", sRead_errmsg);
        free(sRead_errmsg);
        return 1;
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
    return 0;
}

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave){
    
}

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave){
    // 打開劇本檔案
    char *sRead_errmsg = calloc(100, sizeof(char)); // 錯誤訊息
    FILE *fpSave = NULL;
    if ((fpSave = fopen(SavePath, "w")) == NULL)
    {
        perror("Error opening save");
        return 0;
    }
    atexit(fclose(fpSave));

    // 開始進行存檔，使用toml格式
    fprintf(fpSave, "sizePlayerInventory = %ld\n", GameSave->sizePlayerInventory.u.i);
    fprintf(fpSave, "[playerInventory]\n");
    for (int i = 0; i < GameSave->sizePlayerInventory.u.i; i++)
    {
        fprintf(fpSave, "item%d = %s\n", i, GameSave->playerInventory);
    }

    exit(EXIT_SUCCESS);
    
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
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, color, dstRect->w);
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
    SDL_Surface *textSurface = TTF_RenderUTF8_Solid_Wrapped(font, text, color, dstRect->w);
    //SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, text, color);
    //SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, dstRect->w);
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

int8_t DisplayUNICODE(SDL_Renderer *renderer, uint16_t *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect)
{
    // 建立材質
    SDL_Surface *textSurface = TTF_RenderUNICODE_Solid_Wrapped(font, text, color, dstRect->w);
    //SDL_Surface *textSurface = TTF_RenderUNICODE_Solid(font, text, color);
    //SDL_Surface *textSurface = TTF_RenderUNICODE_Blended_Wrapped(font, text, color, dstRect->w);
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

int8_t loadGameSaves;

//int8_t eventHandler(SDL_Renderer *renderer, script_t script, toml_table_t *event, char *option){

//}

