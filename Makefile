CC=gcc -std=gnu99
DEBUG_CFLAGS=-g
SOURCE=src/parse.h src/parse.c src/resources.h src/resources.c src/console.h

all: debug

debug: server-dbg client-dbg

release: server-release client-release

run: release
	/usr/bin/gnome-terminal --window --profile="Persistent" --title="Server" -e ./bin/server; /usr/bin/gnome-terminal --window --profile="Persistent" --title="Client" -e ./bin/client

server-dbg:
	$(CC) $(DEBUG_CFLAGS) -o ./bin/server ./src/server.c $(SOURCE)

client-dbg:
	$(CC) $(DEBUG_CFLAGS) -o ./bin/client ./src/client.c $(SOURCE)

server-release:
	$(CC) -DNDEBUG -o ./bin/server ./src/server.c $(SOURCE)

client-release:
	$(CC) -DNDEBUG -o ./bin/client ./src/client.c $(SOURCE)

clean:
	rm -R bin/*

.PHONY: clean run
