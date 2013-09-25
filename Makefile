CC=gcc -std=gnu99
DEBUG_CFLAGS=-g
SOURCE=src/parse.h src/parse.c src/resources.h src/resources.c src/dbg.h

all: server client

server:
	$(CC) $(DEBUG_CFLAGS) -o bin/server src/server.c $(SOURCE)

client:
	$(CC) $(DEBUG_CFLAGS) -o bin/client src/client.c $(SOURCE)


.PHONY: clean valgrind-parse
