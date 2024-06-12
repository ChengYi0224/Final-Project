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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "toml.h"
#include "toml_extra.h"

typedef struct _sTableScript
{

    toml_datum_t title;
    toml_datum_t author;
    toml_datum_t version;
    toml_datum_t description;
    toml_datum_t license;
    toml_datum_t startBackgroundPath;

    toml_table_t *item;
    toml_table_t *event;
    toml_table_t *scene;
    toml_table_t *dialogue;
    toml_table_t *character;

} script_t;

typedef struct _sScene
{
    toml_datum_t event;
    toml_datum_t background;
    toml_datum_t character;
    toml_datum_t dialogue;
    toml_datum_t effect;
} scene_t;

typedef struct _sGameSave
{
    char SaveName[256];
    toml_table_t *playerInventory; // This is an array
    char event[256];
    scene_t nowScene;
} GameSave_t;

typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    int8_t isHovered;
    int8_t isClicked;
} Button;

extern int64_t gGameVolume;

#define STRING_SAVE_DATUM(datum) "%s=\"%s\"", #datum, datum

#define INT64_SAVE_DATUM(datum) "%s=%ld", #datum, datum

#define DOUBLE_SAVE_DATUM(datum) "%s=%lf", #datum, datum

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define StartBackgroundPathDefault "assets/scenes/defaultBackground.png"
#define imgtest "assets/scenes/2K.jpg"
#define imgtest2 "assets/scenes/aut.jpg"
#define ScriptPath "ta.toml"

int8_t DisplayImg(SDL_Renderer *renderer, char *imgPath, SDL_Rect *srcRect, SDL_Rect *dstRect);

int8_t DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect);

int8_t DisplayUTF8(SDL_Renderer *renderer, uint8_t *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect);

int8_t eventHandler(SDL_Renderer *renderer, script_t script, toml_table_t *event);

int8_t renderButton(SDL_Renderer *renderer, Button *button);

// return value -
// 0: button is not clicked
// 1: button is clicked
int8_t handleButton(SDL_Event *event, Button *button);