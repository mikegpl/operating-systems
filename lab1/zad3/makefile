CC = gcc
CFLAGS = -Wall -std=gnu11
LIBRARY_NAME = contactBook
SHR_FLAGS = -fPIC -shared
INC = -Ilib
SHR_OPTS = -Wl,-rpath=.

LIB_FILES = $(wildcard lib/*.c)
MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(LIB_FILES:.c=.o)))

.PHONY: static, shared, clean

benchmark_static: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -o benchmark

benchmark_shared: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -o benchmark

static: lib$(LIBRARY_NAME).a

shared: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).a: $(OBJ_FILES)
	ar crs $@ $^

lib$(LIBRARY_NAME).so: $(LIB_FILES)
	$(CC) $(SHR_FLAGS) $^ -o $@

bin/%.o: lib/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -rf ./bin/*.o *.a *.so benchmark main