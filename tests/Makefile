CC=gcc
DEBUG_CFLAGS=-g
LDFLAGS=-lrt
SOURCE=../src/parse.h ../src/parse.c ../src/list.h ../src/list.c

all: parse

parse:
	$(CC) -std=gnu99 -o test_parse test_parse.c $(SOURCE)

clean:
	rm -f test_parse
 
.PHONY: clean
