CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lrt

all: server client

server: server.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

server.o: server.cpp
	$(CC) $(CFLAGS) $^ -o $@

client: client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

client.o: client.cpp
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf *.o server client
	