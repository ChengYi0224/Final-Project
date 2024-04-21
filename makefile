EXEC = sre/code/toml.c

ALL:
	gcc -o engine main.c $(EXEC) -lncurses

clean:
	rm *.exe *.o engine