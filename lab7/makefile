CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(MAIN_FILES:.c=.o)))
EXEC_FILES = $(addprefix ./, $(notdir $(MAIN_FILES:.c=.out)))	

.PHONY: clean remake

all: $(EXEC_FILES)

remake: clean all

barber.out: bin/barber.o
	$(CC) $^ -o $@

kamino.out: bin/kamino.o
	$(CC) $^ -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf ./bin/*.o *.out