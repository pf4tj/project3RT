CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -ggdb -O2 -fsanitize=address

.PHONY: clean

clean:
	rm -f *.o