CC=g++
CGFLAGS=-c

all: read-file

read-file: main.o
	$(CC) $^ -o read-file

main.o: main.cpp
	$(CC) $(CGFLAGS) $^ -o main.o

clean:
	rm -rf *.o read-file
