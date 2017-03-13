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
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o
		ar crs libcontact.a ./$(BIN)/libcontact.o 
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -o main -L. -lcontact

clean:
		rm -rf ./bin/*.o *.a main
