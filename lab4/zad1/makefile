CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(MAIN_FILES:.c=.o)))

.PHONY: clean

all: main

main: $(MAIN_FILES)
	$(CC) $(CFLAGS) $^ -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf ./bin/*.o main