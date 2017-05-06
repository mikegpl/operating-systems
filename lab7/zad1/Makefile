CC = gcc
CFLAGS = -std=gnu11 -Wall
BARBER_FILES = src/barber.c src/lib/utils.c src/lib/pidQueue.c
KAMINO_FILES = src/kamino.c src/lib/utils.c src/lib/pidQueue.c

all: kamino barber

barber: $(BARBER_FILES)
	$(CC) $(CFLAGS) $^ -o $@

kamino: $(KAMINO_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm barber kamino

.PHONY: clean all