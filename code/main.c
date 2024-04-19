#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../lib/tomlc99/toml.h"
#include <curses.h>

//#ifdef _WIN32
//#include <ncurses/curses.h> // on Windows
//#elif __linux__
//#include <curses.h>         // on Linux
//#endif 

int main(int argc, char const *argv[])
{
    refresh();
    return 0;    
}
