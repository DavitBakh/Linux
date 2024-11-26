C=g++
CFLAGS=-c

all: copy

copy: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o copy
