all: init client destroy server

init: init.o bank.o
	g++ init.o bank.o -lrt -o init

init.o: init.cpp bank.h
	g++ init.cpp -c -o init.o

bank.o: bank.cpp bank.h
	g++ bank.cpp -c -o bank.o

server: server.o parallel_scheduler.o bank.o
	g++ server.o parallel_scheduler.o bank.o -lrt -o server

server.o: server.cpp
	g++ server.cpp -c -o server.o

destroy: destroy.o
	g++ destroy.o -lrt -o destroy

destroy.o: destroy.cpp
	g++ destroy.cpp -c -o destroy.o

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	g++ parallel_scheduler.cpp -c -o parallel_scheduler.o

clean:
	rm -rf *.o init server client destroy

client: client.o bank.o
	g++ client.o bank.o -lrt -o client

client.o: client.cpp
	g++ client.cpp -c -o client.o