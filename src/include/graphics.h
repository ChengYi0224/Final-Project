#pragma once
#include "control.h"

int8_t updateInventory(SDL_Renderer *renderer, const GameSave_t saving);

int8_t updateScene(SDL_Renderer *renderer, const GameSave_t saving);

int8_t updateAvatar(SDL_Renderer *renderer, const GameSave_t saving);

int8_t updateTachie(SDL_Renderer *renderer, const GameSave_t saving);