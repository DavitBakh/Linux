C=g++
CFLAGS=-c

all: interactive-shell

interactive-shell: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o interactive-shell