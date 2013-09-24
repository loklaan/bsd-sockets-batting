CC=gcc -std=gnu99
DEBUG_CFLAGS=-g -Wall
LDFLAGS=-lrt
SOURCE=../src/parse.h ../src/parse.c ../src/resources.h ../src/resources.c

all: parse

parse:
	$(CC) $(DEBUG_CFLAGS) -o test_parse test_parse.c $(SOURCE)

clean:
	rm -f test_parse

valgrind-parse: parse
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./test_parse

.PHONY: clean valgrind-parse
