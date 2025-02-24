C=g++
CFLAGS=-c -lpthread -lrt

all: server client

server: server.o
	$(C) $^ -o $@

server.o: server.cpp
	$(C) $(CFLAGS) $^ -o $@

client: client.o
	$(C) $^ -o $@

client.o: client.cpp
	$(C) $(CFLAGS) $^ -o $@


clean:
	rm -rf *.o server client
	