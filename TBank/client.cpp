#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"

int main(int argc, char **argv)
{

  int shm_fd = shm_open("/TransparentBank", O_RDWR, 0666);
  struct stat sb;
  if (fstat(shm_fd, &sb) == -1)
  {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  int shm_size = sb.st_size;
  void *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  Bank *bank = (Bank *)shm_ptr;

  bank->print();
  bank->printBalance(0);
  bank->printMinBalance(0);
  bank->printMaxBalance(0);
  bank->froze_defroze(0);
  bank->froze_defroze(0);
  bank->creditToAll(500);



  munmap(shm_ptr, shm_size);
  close(shm_fd);
}
