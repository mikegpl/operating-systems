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

measurement:
	./measurement.sh

benchmark_static: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -o benchmark

benchmark_shared: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -o benchmark

benchmark_dynamic: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -o benchmark


# optimalization strategies
# static lib
benchmark_staticO0: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -O0 -o benchmark

benchmark_staticO1: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -O1 -o benchmark

benchmark_staticO2: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -O2 -o benchmark

benchmark_staticOs: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -Os -o benchmark

benchmark_staticO3: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -O3 -o benchmark

benchmark_staticOfast: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -Ofast -o benchmark

# shared lib
benchmark_sharedO0: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -O0 -o benchmark

benchmark_sharedO1: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -O1 -o benchmark

benchmark_sharedO2: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -O2 -o benchmark

benchmark_sharedOs: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -Os -o benchmark

benchmark_sharedO3: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -O3 -o benchmark

benchmark_sharedOfast: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -L. -l$(LIBRARY_NAME) -Ofast -o benchmark

# dynamic lib

benchmark_dynamicO0: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -O0 -o benchmark

benchmark_dynamicO1: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -O1 -o benchmark

benchmark_dynamicO2: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -O2 -o benchmark

benchmark_dynamicOs: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -Os-o benchmark

benchmark_dynamicO3: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -O3 -o benchmark

benchmark_dynamicOfast: shared
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) $(SHR_OPTS) -ldl -DDYNAMIC_LOADING -Ofast -o benchmark




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