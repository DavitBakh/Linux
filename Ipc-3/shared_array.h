#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <csignal>
#include <string>

class SharedArray 
{
private:
	int* data;
	std::string name;
	std::size_t size;
	int shFd;

public:
	SharedArray(std::string name_, std::size_t size_): name(name_), size(size_) 
    {
		shFd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
		if (shFd == -1) 
        {
			perror("Shm open error!");
			exit(errno);
		}

		if (ftruncate(shFd, size * sizeof(int)) == -1) {
			perror("Set size error!");
			exit(errno);
		}

		data = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shFd, 0));
		if (data == MAP_FAILED) {

			perror("Mmap error!");
			exit(errno);
		}

	}

	int& operator[](std::size_t index) 
    {
		if (index < size)
			return data[index];
        return errno;
	}

	~SharedArray() 
    {
		munmap(data, size * sizeof(int));
		close(shFd);
		shm_unlink(name.c_str());
	}

};



#endif