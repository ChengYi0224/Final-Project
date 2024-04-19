ALL:
	gcc -o main code/main.c -lncurses

clean:
	rm *.exe *.o main