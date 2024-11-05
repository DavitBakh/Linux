CC=g++
CGFLAGS=-c

all: simple-copy

simple-copy: main.o
	$(CC) $^ -o $@

main.o: main.cpp
	$(CC) $(CGFLAGS) $^ -o $@

clean:
	rm -rf *.o simple-copy
