all: init client destroy

init: init.o bank.o
	g++ init.o bank.o -lrt -o init

init.o: init.cpp bank.h
	g++ init.cpp -c -o init.o

bank.o: bank.cpp bank.h
	g++ bank.cpp -c -o bank.o

client: client.o bank.o
	g++ client.o bank.o -lrt -o client

client.o: client.cpp
	g++ client.cpp -c -o client.o

destroy: destroy.o
	g++ destroy.o -lrt -o destroy

destroy.o: destroy.cpp
	g++ destroy.cpp -c -o destroy.o

clean:
	rm -rf *.o init client destroy
