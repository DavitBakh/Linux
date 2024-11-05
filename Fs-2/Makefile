CC=g++
CGFLAGS=-c

all: simple-copy

simple-copy: main.o
	$(CC) $^ -o simple-copy

main.o: main.cpp
	$(CC) $(CGFLAGS) $^ -o main.o

clean:
	rm -rf *.o simple-copy
