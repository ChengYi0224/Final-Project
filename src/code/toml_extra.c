#include "../include/toml.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void toml_set_string(toml_datum_t *datum, const char *str)
{
    datum->u.s = strdup(str); // 賦值新字符串，strdup會分配內存
    datum->ok = 1;            // 標記為有效
}

void toml_set_int(toml_datum_t *datum, int64_t value)
{
    datum->u.i = value;
    datum->ok = 1; // 標記為有效
}

void toml_set_double(toml_datum_t *datum, double value)
{
    datum->u.d = value;
    datum->ok = 1; // 標記為有效
}

void toml_set_bool(toml_datum_t *datum, int value)
{
    datum->u.b = value;
    datum->ok = 1; // 標記為有效
}

// 自動分配字串記憶體
char *my_strdup(const char *str)
{
    if (str == NULL)
        return NULL;

    char *copy = (char *)malloc(strlen(str) + 1);
    if (copy == NULL)
        return NULL;

    strcpy(copy, str);

    return copy;
}