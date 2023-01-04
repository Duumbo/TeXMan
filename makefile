.PHONY: all, compile, clean

all: compile

compile: src/main.c
	clang src/main.c -o output/build

clean:
	rm output/*

