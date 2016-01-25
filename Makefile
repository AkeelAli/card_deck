CC=gcc
CFLAGS= -Wall -g

all: rounds

rounds: rounds.c deck.o rotation.o
	$(CC) -o $@ $(CFLAGS) $(MACROS) $^

rotation.o: rotation.c
	$(CC) -c -o $@ $(CFLAGS) $(MACROS) $^

deck.o: deck.c
	$(CC) -c -o $@ $(CFLAGS) $^


.PHONY: clean

clean:
	rm -rf *.o rounds *.dSYM
