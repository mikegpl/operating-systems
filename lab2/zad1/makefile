CC = gcc
CFLAGS = -Wall -std=gnu11
LIBRARY_NAME = ioOperations
INC = -Ilib

LIB_FILES = $(wildcard lib/*.c)
MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(LIB_FILES:.c=.o)))

.PHONY: static, clean

all: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -o main

static: lib$(LIBRARY_NAME).a

lib$(LIBRARY_NAME).a: $(OBJ_FILES)
	ar crs $@ $^

bin/%.o: lib/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -rf ./bin/*.o *.a main