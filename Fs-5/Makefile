C=g++
CFLAGS=-c

all: manual-append

manual-append: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o manual-append
