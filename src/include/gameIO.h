#include "control.h"
#pragma once

int8_t GameStartMenu(SDL_Renderer *renderer, script_t *mainScript, GameSave_t *saving);

int8_t GameSaveRead(char *SavePath, GameSave_t *GameSave);

int8_t GameSaveWrite(char *SavePath, GameSave_t *GameSave);

int8_t startNewGame(script_t *mainScript, GameSave_t *saving);

NEXT_ACTION eventHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving);

NEXT_ACTION dialogueHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving);

NEXT_ACTION optionHandler(SDL_Renderer *renderer, script_t *script, GameSave_t *saving, int32_t optionIdx);

//  return 0 when succeeded
//  return 1 when failed
int8_t scriptRead(char *scriptPath, script_t *script);