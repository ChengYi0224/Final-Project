#ifdef SCRIPTPARSE_H
#define SCRIPTPARSE_H

#include "toml.h"

typedef struct
{
    char* id;
    char* title;
    char* description;
    char* background;
    int option_count;
    Option* options;
} Scene;

typedef struct
{
    char* name;
    char* description;
    char* image;
} Character;

typedef struct
{
    char* name;
    char* description;
    char* image;
    char* use_effect;
} Item;

typedef struct
{
    char *title;
    char *author;
    char *description;
    Scene* scenes;
    int scene_count;
    Character* characters;
    int character_count;
    Item* items;
    int item_count;
} GameConfig;

GameConfig* parse_toml(char* filename);
void free_game_config(GameConfig* config);

#endif