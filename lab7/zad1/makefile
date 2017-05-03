CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard src/*.c)
LIB_FILES = $(wildcard src/lib/bin/*.o)
OBJ_FILES = $(addprefix bin/, $(notdir $(MAIN_FILES:.c=.o)))
EXEC_FILES = $(addprefix ./, $(notdir $(MAIN_FILES:.c=.out)))	

.PHONY: clean remake

all: $(EXEC_FILES)

remake: clean all

%.out: bin/%.o
	$(CC) $^ $(LIB_FILES) -o $@	

$(OBJ_FILES): $(MAIN_FILES)
	$(CC) $(CFLAGS) $< -c -o $@ 

clean:
	rm -rf ./bin/*.o *.out