#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"

int main(int argc, char** argv) {

  int shm_fd = shm_open("/TransparentBank", O_RDWR, 0666);
  struct stat sb;
  if (fstat(shm_fd, &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
  }

  int shm_size = sb.st_size;
  std::cout << shm_size << std::endl; 
  void* shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  std::cout << 1 << std::endl;
  Bank* bank = (Bank*)shm_ptr;
 std::cout << 1 << std::endl;
  bank->print();
std::cout << 1 << std::endl;
  munmap(shm_ptr, shm_size);
  close(shm_fd);
	  
}

