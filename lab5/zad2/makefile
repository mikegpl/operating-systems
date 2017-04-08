CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard src/*.c)
EXEC_FILES = $(notdir $(MAIN_FILES:.c=))
OBJ_FILES = $(addprefix bin/, $(notdir $(MAIN_FILES:.c=.o)))

.PHONY: clean

all: $(EXEC_FILES)

master: bin/master.o
	$(CC) $(CFLAGS) $^ -o $@	

slave: bin/slave.o
	$(CC) $(CFLAGS) $^ -lm -o $@	

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf ./bin/*.o $(EXEC_FILES) *.p