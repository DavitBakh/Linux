C=g++
FLAGS=-c

all: hacker-rm

hacker-rm: main.o
	$(C) $^ -o $@

main.o: main.cpp
	$(C) $(FLAGS) $^ -o $@

clean:
	rm -rf *.o hacker-rm
