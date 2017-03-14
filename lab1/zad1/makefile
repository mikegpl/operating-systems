CC = gcc
CFLAGS = -Wall -pedantic -std=c11
LIB = lib
BIN = bin
INC = -I$(LIB)
SHRLIBOPT = -fPIC -shared
.PHONY: static, shared, clean
 

static: clean
		$(CC) $(CFLAGS) $(INC) -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o
		ar crs libcontact.a ./$(BIN)/libcontact.o 

shared: clean
		$(CC) $(CFLAGS) -fPIC -c ./$(LIB)/libcontact.c -o ./$(BIN)/libcontact.o
		$(CC) $(SHRLIBOPT) ./$(BIN)/libcontact.o -o ./libcontactbook.so

clean:
		rm -rf ./bin/*.o *.a *.so