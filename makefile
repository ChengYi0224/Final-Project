# C source code
SRC_FILE = sre/code/*.c

# Output Executable
EXEC = engine

# SDL Library
SDL_LIB = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

ALL:
	gcc -o $(EXEC) main.c $(SRC_FILE) $(SDL_LIB)

clean:
	rm *.exe *.o $(EXEC)