# Makefile for things and kittens.

CC=@gcc -std=gnu99 $(LFLAGS)
LFLAGS=-pthread
DFLAGS=-g
SOURCE=src/parse.h src/parse.c src/resources.h src/resources.c src/console.h
CLIENTS=1

all: debug

# quick execute target. change 'all' req to debug/release.
# usage:
# make run <CLIENT=#>
run: all
	@/usr/bin/gnome-terminal --window --title="Server" -e ./bin/server; \
	bash -c \
	"\
		for (( i=0; i<$(CLIENTS); i++ )) ; do \
			/usr/bin/gnome-terminal --window --title=Client -e ./bin/client ; \
		done\
	"

debug: server-dbg client-dbg

release: server-release client-release

server-dbg:
	$(CC) $(DFLAGS) -o ./bin/server ./src/server.c $(SOURCE)

client-dbg:
	$(CC) $(DFLAGS) -o ./bin/client ./src/client.c $(SOURCE)

server-release:
	$(CC) -DNDEBUG -o ./bin/server ./src/server.c $(SOURCE)

client-release:
	$(CC) -DNDEBUG -o ./bin/client ./src/client.c $(SOURCE)

clean:
	rm -R bin/*

.PHONY: clean run
