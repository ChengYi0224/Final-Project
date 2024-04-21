#ifdef SCRIPTPARSE_H
#define SCRIPTPARSE_H

#include "toml.h"

typedef struct
{
    char *id;
    char *title;
    char *description;
    char *background;
    int option_count;
    struct option* options;
} Scene;

typedef struct
{
    char *text;
    char *next_scene;
} Option;

typedef struct
{
    char *title;
    char *author;
    char *description;
    Scene* scenes;
    int scene_count;
} GameConfig;

GameConfig* parse_toml(char* filename);
void free_game_config(GameConfig* config);

#endif