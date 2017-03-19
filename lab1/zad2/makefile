CC = gcc
CFLAGS = -Wall -std=gnu11
LIBRARY_NAME = contactBook
SHRFLAGS = -fPIC -shared
INC = -Ilib

LIB_FILES = $(wildcard lib/*.c)
MAIN_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix bin/, $(notdir $(LIB_FILES:.c=.o)))

.PHONY: static, shared, clean

main_static: static
	$(CC) $(CFLAGS) $(INC) mainsrc/main.c -L. -l$(LIBRARY_NAME) -o main

main_shared: shared
	$(CC) $(CFLAGS) $(INC) mainsrc/main.c -Wl,-rpath=. -L. -l$(LIBRARY_NAME) -o main

benchmark_static: static
	$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -l$(LIBRARY_NAME) -o benchmark



static: lib$(LIBRARY_NAME).a

shared: lib$(LIBRARY_NAME).so

lib$(LIBRARY_NAME).a: $(OBJ_FILES)
	ar crs $@ $^

lib$(LIBRARY_NAME).so: $(LIB_FILES)
	$(CC) $(SHRFLAGS) $^ -o $@

bin/%.o: lib/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# shared: clean
# 		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o
# 		$(CC) $(SHR_LIB_OPT) ./$(BIN)/libcontact.o -o ./libcontact.so
# 		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -Wl,-rpath=. -L. -lcontact -o main

# benchmark_static: clean
# 		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o 
# 		ar crs libcontact.a ./$(BIN)/libcontact.o  
# 		$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -lcontact -o benchmark

# benchmark_shared: clean
# 		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o
# 		$(CC) $(SHR_LIB_OPT) ./$(BIN)/libcontact.o -o ./libcontact.so
# 		$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -Wl,-rpath=. -L. -lcontact -o benchmark
clean:
		rm -rf ./bin/*.o *.a *.so benchmark main