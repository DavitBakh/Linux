C=g++
CFLAGS=-c

all: do-command

do-command: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o do-command
