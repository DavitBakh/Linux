C=g++
CFLAGS=-c

all: prime-calculator

prime-calculator: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o prime-calculator