C=g++
CFLAGS=-c

all: signal-echo

signal-echo: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o signal-echo
