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

typedef struct _sTableScript
{
    toml_table_t *rootTable; // Please toml_free() when exit

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
    toml_datum_t scene;
    toml_datum_t character;
    toml_datum_t dialogue;
    toml_datum_t effect;
} scene_t;

typedef struct _sGameSave
{
    char SaveName[256];
    toml_array_t *playerInventory; // This is an array
    scene_t nowScene;
    toml_table_t *tabCurEvent;
    toml_table_t *tabCurDialogue;
} GameSave_t;

typedef struct
{
    SDL_Rect rect;
    SDL_Color color;
    int8_t isHovered;
    int8_t isClicked;
} Button;

typedef enum _eNextAction
{
    _eEMPTY,
    _eDIALOGUE,
    _eEVENT,
    _eENDING
} NEXT_ACTION;

#include "toml_extra.h"
#include "gameIO.h"
#include "graphics.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#ifndef BASIC_GLOBAL_VARIABLES
#define BASIC_GLOBAL_VARIABLES
extern int64_t gGameVolume;
extern TTF_Font *gFontDefault;
extern SDL_Color gColorWHITE;
extern SDL_Color gColorBLACK;
extern SDL_Color gColorGREY;
extern SDL_Color gColorLGREY;
extern SDL_Color gColorDGREY;
extern SDL_Color gColorRED;
extern SDL_Color gColorGREEN;
extern SDL_Color gColorBLUE;

extern SDL_Color gColorDialogue;
extern SDL_Color gColorBackground;
extern SDL_Color gColorOptionButton;

extern SDL_Rect gRectDialogue;
extern SDL_Rect gRectBackground;
extern SDL_Rect gRectTachie;
extern SDL_Rect gRectText;
extern SDL_Rect gRectAvatar;
extern SDL_Rect gRectNext;
extern SDL_Rect gRectOption[4];

extern SDL_Rect gRectInventory[4];

extern toml_table_t *gRootTabGameSaveRead;
#endif

#define TOML_USE_STRING(toml_datum) toml_datum.ok == 1 ? toml_datum.u.s : ""

#define TOML_USE_INT(toml_datum) toml_datum.ok == 1 ? toml_datum.u.i : 0

#define TOML_USE_FLOAT(toml_datum) toml_datum.ok == 1 ? toml_datum.u.f : 0.0

#define SET_DRAW_COLOR(renderer, SDL_color) SDL_SetRenderDrawColor(renderer, SDL_color.r, SDL_color.g, SDL_color.b, SDL_color.a)

#define StartBackgroundPathDefault "assets/scenes/defaultBackground.png"
#define imgtest "assets/scenes/2K.jpg"
#define imgtest2 "assets/scenes/aut.jpg"
#define ScriptPath "script.toml"

int8_t DisplayImg(SDL_Renderer *renderer, char *imgPath, SDL_Rect *srcRect, SDL_Rect *dstRect);

int8_t DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect);

int8_t DisplayUTF8(SDL_Renderer *renderer, uint8_t *text, TTF_Font *font, SDL_Color color, SDL_Rect *dstRect);

int8_t renderButton(SDL_Renderer *renderer, Button *button);

// return value -
// 0: button is not clicked
// 1: button is clicked
int8_t handleButton(SDL_Event *event, Button *button);

// example : struct tm *timeinfo;
// use asctime(timeinfo) to turn into string.
struct tm *getLocalTime();

int8_t setCentre(SDL_Rect *window, SDL_Rect *rect);