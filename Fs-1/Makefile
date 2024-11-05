CC=g++
CGFLAGS=-c

all: read-file

read-file: main.o
	$(CC) $^ -o $@

main.o: main.cpp
	$(CC) $(CGFLAGS) $^ -o $@

clean:
	rm -rf *.o read-file
