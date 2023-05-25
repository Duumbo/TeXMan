.PHONY: all, compile, clean

CFLAGS = -Wall -g
CC = clang

all: compile

compile: src/main.c
	${CC} ${CFLAGS} src/main.c -o output/build

clean:
	rm output/*

