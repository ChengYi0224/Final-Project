# C source code
SRC_CFILE = src/code/*.c

# Output Executable
EXEC = engine

# SDL Library
# complete version: SDL_LIB = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
SDL_LIB = -lSDL2 -lSDL2_image -lSDL2_ttf

ALL:
	gcc -o $(EXEC) main.c $(SRC_CFILE) $(SDL_LIB)

clean:
	rm *.exe *.o $(EXEC)