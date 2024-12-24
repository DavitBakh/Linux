C=g++
CFLAGS=-c

all: array-summary

array-summary: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o array-summary
	