# weird.js makefile

CC = gcc

all:
	$(CC) -g3 -o weird compiler.c

clean:
	rm weird
