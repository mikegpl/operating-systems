CC = gcc
CFLAGS = -Wall -std=c11
SRC = src
LIB = lib
BIN = bin
INC = -I$(LIB)
SHRLIBOPT = -fPIC -shared
.PHONY: static, shared, clean
 
static: clean
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o 
		ar crs libcontact.a ./$(BIN)/libcontact.o  
		$(CC) $(CFLAGS) $(INC) ./$(SRC)/main.c -o main -L. -lcontact
		./main

shared: clean
		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o
		$(CC) $(SHRLIBOPT) ./$(BIN)/libcontact.o -o ./libcontactbook.so
		$(CC) $(INC) ./$(SRC)/main.c -L. -Wl,-rpath=. -lcontactbook -o main
		./main

clean:
		rm -rf ./bin/*.o *.a *.so main