CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lrt

all: server client

server: server.o parallel_scheduler.o
	$(CC) $^ -o $@ $(LDFLAGS)

server.o: server.cpp
	$(CC) $(CFLAGS) $^ -o $@

client: client.o
	$(CC) $^ -o $@ $(LDFLAGS)

client.o: client.cpp
	$(CC) $(CFLAGS) $^ -o $@

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o server client
	