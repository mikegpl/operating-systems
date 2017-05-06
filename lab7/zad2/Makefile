CC = gcc
CFLAGS = -std=gnu11 -Wall
BARBER_FILES = src/barber.c src/lib/utils.c src/lib/pidQueue.c
KAMINO_FILES = src/kamino.c src/lib/utils.c src/lib/pidQueue.c
LINKING_OPTIONS = -lrt -lpthread

all: kamino barber

barber: $(BARBER_FILES)
	$(CC) $(CFLAGS) $^ $(LINKING_OPTIONS) -o $@

kamino: $(KAMINO_FILES)
	$(CC) $(CFLAGS) $^ $(LINKING_OPTIONS) -o $@

clean:
	rm barber kamino

.PHONY: clean all