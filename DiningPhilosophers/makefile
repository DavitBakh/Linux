C=g++
CFLAGS=-c -lpthread -lrt

all: dining_philosophers

dining_philosophers: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o dining_philosophers
	