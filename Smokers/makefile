CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lrt

all: smokers

smokers: main.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o smokers