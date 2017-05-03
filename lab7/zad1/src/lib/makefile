CC = gcc
CFLAGS = -Wall -std=gnu11

MAIN_FILES = $(wildcard ./*.c)
OBJ_FILES = $(addprefix ./, $(notdir $(MAIN_FILES:.c=.o)))	

.PHONY: clean remake

all: $(OBJ_FILES)

remake: clean all

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ 

clean:
	rm -f *.o