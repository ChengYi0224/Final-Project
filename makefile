# C source code
SRC_CFILE = ./src/code/*.c

#C header file
INCLUDE_HFILE = -I./src/include/

# Output Executable
EXEC = engine
EXEC_TEST = es

# SDL Library
SDL_LIB = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

ALL:
	gcc -o $(EXEC) main.c $(SRC_CFILE) $(SDL_LIB) $(INCLUDE_HFILE)

debug:
	gcc -g -o $(EXEC) main.c $(SRC_CFILE) $(SDL_LIB) $(INCLUDE_HFILE)

install-SDL:
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

test:
	gcc -g -o $(EXEC_TEST) test.c $(SDL_LIB) $(INCLUDE_HFILE)

clean:
	rm -f $(EXEC) $(EXEC_TEST)