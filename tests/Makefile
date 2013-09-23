CC=gcc -std=gnu99
DEBUG_CFLAGS=-g
LDFLAGS=-lrt
SOURCE=../src/parse.h ../src/parse.c ../src/list.h ../src/list.c

all: parse

parse:
	$(CC) $(DEBUG_CFLAGS) -o test_parse test_parse.c $(SOURCE)

clean:
	rm -f test_parse
 
.PHONY: clean
