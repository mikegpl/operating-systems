CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(MAIN_FILES:.c=.o)))
EXEC_FILES = server client

.PHONY: clean 

all: $(EXEC_FILES)

server: bin/server.o
	$(CC) $^ -o $@ -lrt

client: bin/client.o
	$(CC) $^ -o $@ -lrt

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf ./bin/*.o $(EXEC_FILES)