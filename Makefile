SRC += *.c
build:
	gcc $(SRC) -std=c99 -Wall -lSDL3 -o hello_world

run:
	./hello_world

