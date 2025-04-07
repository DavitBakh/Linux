all: init client

init: init.o bank.o
	g++ init.o bank.o -lrt -o init -g

client: client.o bank.o
	g++ client.o bank.o -lrt -o client -g

client.o: client.cpp
	g++ client.cpp -c -o client.o -g

init.o: init.cpp bank.h
	g++ init.cpp -c -o init.o -g

bank.o: bank.cpp bank.h
	g++ bank.cpp -c -o bank.o -g

clean:
	rm -rf *.o init
