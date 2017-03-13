CC = gcc
CFLAGS = -Wall -pedantic -std=c11
SRC = src
LIB = lib
BIN = bin
INC = -I$(LIB)
.PHONY: all, static, shared, dynamic, clean

all: clean static

static:
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o
		ar crs libcontact.a ./$(BIN)/libcontact.o 
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -o main -L. -lcontact

clean:
		rm -rf ./bin/*.o *.a main