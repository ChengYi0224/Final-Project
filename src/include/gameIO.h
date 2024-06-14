#include "control.h"
#pragma once

toml_table_t *GameStartMenu(SDL_Renderer *renderer, script_t *mainScript, GameSave_t *saving);

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave);

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave);

int8_t startNewGame(script_t *mainScript, GameSave_t *saving);

// return value: next event( toml_table_t* )
int8_t eventHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving,toml_table_t *event);

//  return 0 when succeeded
//  return 1 when failed
int8_t scriptRead(char *scriptPath, script_t *script);