/*
    程式包含以下功能:
    -   滑鼠 / 鍵盤輸入
    -   介面顯示
        -   台詞顯示
        -   背景顯示
        -   頭像顯示
        -   特效
    -   從script.toml讀取劇本
        1.  讀取劇本
        2.  分類物件
        3.  依序執行
*/
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/control.h"

void scriptRun(FILE *fpScript){
    char *errmsg = calloc( 100, sizeof(char) ); // 錯誤訊息
    // 讀取劇本
    toml_table_t *wholeScript   = toml_parse_file(fpScript, errno, 100);
    if(!wholeScript){
        perror("Script parsing failed\n");
        return;
    }
    // 讀取標頭資料
    toml_datum_t title  = toml_string_in(wholeScript, "title");
    toml_datum_t author = toml_string_in(wholeScript, "author");
    //toml_datum_t ver    = toml_string_in(wholeScript, "version");
    //toml_datum_t description = toml_string_in(wholeScript, "description");
    //toml_datum_t license = toml_string_in(wholeScript, "license");

    // 分類物件
    toml_table_t *tableEvent    = toml_table_in(wholeScript, "event");
    toml_table_t *tableScene    = toml_table_in(wholeScript, "scene");
    toml_table_t *tableDialogue = toml_table_in(wholeScript, "dialogue");
    toml_table_t *tableCharacter = toml_table_in(wholeScript, "character");

    // 執行event.start
    toml_table_t *tableStart = toml_table_in(tableEvent, "start");

    // debugging
    toml_datum_t next = toml_string_in(tableEvent, "next");
    printf("%s", next.u.s);

    // 釋放資源
    toml_free(wholeScript);
    toml_free(tableEvent);
    toml_free(tableScene);
    toml_free(tableDialogue);
    toml_free(tableCharacter);
    toml_free(tableStart);
    fclose(fpScript);
    free(errmsg);
    

}

void DisplayImg(SDL_Renderer *renderer, char *imgPath){
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
