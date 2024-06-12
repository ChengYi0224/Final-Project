#include "control.h"
#pragma once

toml_table_t *GameStartMenu(SDL_Renderer *renderer, script_t mainScript);

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave);

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave);

//  return 0 when succeeded
//  return 1 when failed
int8_t scriptRead(char *scriptPath, script_t *script);