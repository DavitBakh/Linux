C=g++

all: parallel_scheduler.so demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(C) $< -std=c++17 -Wall -pthread -fPIC -c

parallel_scheduler.so: parallel_scheduler.o
	$(C) $^ -shared -o $@

demo-application: demo.cpp parallel_scheduler.so
	$(C) $^ -std=c++17 -Wall -pthread -fPIC -Wl,-rpath,. -o $@

clean:
	rm *.o *.so demo-application