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
#pragma once
#include "toml.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct tableScript
{

    toml_datum_t title;
    toml_datum_t author;
    toml_datum_t ver;
    toml_datum_t description;
    toml_datum_t license;

    toml_table_t *event;
    toml_table_t *scene;
    toml_table_t *dialogue;
    toml_table_t *character;

} script_t;

void scriptRun(FILE *fpScript);

void DisplayImg(SDL_Renderer *renderer, char *imgPath);

// display text with font
void DisplayText(SDL_Renderer *renderer, );

void eventHandler(SDL_Renderer *renderer, toml_table_t *event);
