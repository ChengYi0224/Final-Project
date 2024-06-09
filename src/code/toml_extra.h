#pragma once
#include "../include/toml.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void toml_set_string(toml_datum_t *datum, const char *str)
{
    if (datum->u.s)
    {
        free(datum->u.s); // 先釋放舊的字符串內存
    }
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

#ifndef __cplusplus__strings__
char *strdup(const char *str)
{
    if (str == NULL)
        return NULL;

    char *copy = (char *)malloc(strlen(str) + 1);
    if (copy == NULL)
        return NULL;

    strcpy(copy, str);

    return copy;
}
#endif