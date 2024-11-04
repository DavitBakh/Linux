CC=g++
CGFLAGS=-c

all: program

program: main.o complex.o sort.o
	$(CC) $^ -o $@

main.o: main.cpp complex.h sort.h
	$(CC) $(CGFLAGS) $< -o $@

complex.o: complex.cpp complex.h
	$(CC) $(CGFLAGS) $< -o $@

sort.o: sort.cpp sort.h complex.h
	$(CC) $(CGFLAGS) $< -o $@

clean:
	rm -rf *.o program
