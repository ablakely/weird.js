# wtf.js makefile

CC = gcc

all:
	$(CC) -g3 -o wtf compiler.c

clean:
	rm wtf.js
