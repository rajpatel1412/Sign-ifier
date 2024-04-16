CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean

all: read_inference

c_program: read_inference.o
	$(CC) $(CFLAGS) -o $@ $<

c_program.o: read_inference.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f read_inference read_inference.o