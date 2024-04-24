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

int8_t scriptRun(FILE *fpScript, script_t *script){
    char *errmsg = calloc( 100, sizeof(char) ); // 錯誤訊息
    // 讀取劇本
    toml_table_t *wholeScript   = toml_parse_file(fpScript, errmsg, 100);
    if(!wholeScript){
        printf("Script parsing failed: %s\n", errmsg);
        return;
    }
    // 讀取標頭資料
    (*script).title     = toml_string_in(wholeScript, "name");
    (*script).author    = toml_string_in(wholeScript, "author");
    (*script).version   = toml_string_in(wholeScript, "version");
    (*script).license   = toml_string_in(wholeScript, "license");
    (*script).description = toml_string_in(wholeScript, "description");

    // 分類物件
    (*script).event     = toml_table_in(wholeScript, "event");
    (*script).scene     = toml_table_in(wholeScript, "scene");
    (*script).dialogue  = toml_table_in(wholeScript, "dialogue");
    (*script).character = toml_table_in(wholeScript, "character");

    // 釋放資源
    toml_free(wholeScript);
    fclose(fpScript);
    free(errmsg);
    printf("Here\n");
}

int8_t DisplayImg(SDL_Renderer *renderer, char *imgPath){
    // 讀取圖片
    SDL_Surface *image = IMG_Load(imgPath);
    // 建立材質
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    // 顯示
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    // 釋放資源
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

int8_t DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color,
                    int32_t x, int32_t y, int32_t w, int32_t h ){
    // 建立材質
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (texture == NULL)
    {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);

    return 1;
}

int8_t eventHandler(SDL_Renderer *renderer, toml_table_t *event){

}
