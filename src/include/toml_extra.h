#include "toml.h"
#include "control.h"
#pragma once

void toml_set_string(toml_datum_t *datum, const char *str);

void toml_set_int(toml_datum_t *datum, int64_t value);

void toml_set_double(toml_datum_t *datum, double value);

void toml_set_bool(toml_datum_t *datum, int value);

// 自動分配字串記憶體
char *my_strdup(const char *str);