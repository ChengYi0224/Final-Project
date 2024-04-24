#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "src/include/toml.h"
#include "toml_cat.c"

int main(int argc, char const *argv[])
{
    toml_table_t *wholeScript = toml_parse_file(fopen("script.toml", "r"), NULL, 0);
    print_table(wholeScript);

    return 0;
}
