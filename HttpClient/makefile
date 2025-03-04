C=g++
CFLAGS=-c -lpthread -lrt

all: client

client: client.o
	$(C) $^ -o $@

client.o: client.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o client
	