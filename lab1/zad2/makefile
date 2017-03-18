CC = gcc
CFLAGS = -Wall -std=gnu11
SRC = src
LIB = lib
BIN = bin
INC = -I$(LIB)
SHR_LIB_OPT = -fPIC -shared
MAIN_FILES = ./$(SRC)/benchmark.c ./$(SRC)/measureTime.c

.PHONY: static, shared, clean

static: clean
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o 
		ar crs libcontact.a ./$(BIN)/libcontact.o  
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -L. -lcontact -o main

shared: clean
		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o
		$(CC) $(SHR_LIB_OPT) ./$(BIN)/libcontact.o -o ./libcontact.so
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -Wl,-rpath=. -L. -lcontact -o main

benchmark_static: clean
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o 
		ar crs libcontact.a ./$(BIN)/libcontact.o  
		$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -L. -lcontact -o benchmark

benchmark_dynamic: clean
		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/contact.c -o ./$(BIN)/libcontact.o
		$(CC) $(SHR_LIB_OPT) ./$(BIN)/libcontact.o -o ./libcontact.so
		$(CC) $(CFLAGS) $(INC) $(MAIN_FILES) -Wl,-rpath=. -L. -lcontact -o benchmark
clean:
		rm -rf ./bin/*.o *.a *.so benchmark main