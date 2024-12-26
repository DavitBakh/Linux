C=g++
CFLAGS=-c

all: first second

first: first.cpp
	$(C) $^ -o $@  -lrt -lpthread

second: second.cpp
	$(C) $^ -o $@ -lrt -lpthread

clean:
	rm -rf *.o first second

