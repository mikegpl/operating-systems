CC = gcc
CFLAGS = -Wall -pedantic -std=c11
SRC = src
LIB = lib
BIN = bin
INC = -I$(LIB)
.PHONY: static
.PHONY: shared
.PHONY: dynamic
.PHONY: clean


static:
		$(CC) $(CFLAGS) -c ./$(LIB)/liblist.c -o ./$(BIN)/liblist.o
		ar crs liblist.a ./$(BIN)/liblist.o 
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -o main -L. -llist

clear:
		rm -rf ./bin/*.o *.a main
