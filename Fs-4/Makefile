C=g++
CFLAGS=-c

all: redirect-cin

redirect-cin: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o redirect-cin
	
