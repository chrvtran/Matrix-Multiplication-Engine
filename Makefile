all: bench interactive test

CC=gcc
CFLAGS=-I. -lm

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

bench: bench.o matrix.o
	$(CC) -o $@ $^ $(CFLAGS)

interactive: interactive.o scheduler.o matrix.o
	$(CC) -o $@ $^ $(CFLAGS)

test: test.o scheduler.o matrix.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f interactive
	rm -f test
	rm -f bench