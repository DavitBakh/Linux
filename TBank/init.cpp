#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "bank.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "Argument count error" << std::endl;
    return -1;
  }

  sem_t* sem = sem_open("/TBankSem", O_CREAT , 0666, 1);
  if (sem == SEM_FAILED)
  {
    std::cout << "error of sem_open";
    exit(errno);
  }

  sem_wait(sem);

  int n = atoi(argv[1]);
  int shm_fd = shm_open("/TBank", O_CREAT | O_TRUNC | O_RDWR, 0666);
  int shm_size = sizeof(Bank) + sizeof(Bill) * n;
  if (ftruncate(shm_fd, shm_size) < 0)
  {
    std::cout << "error of ftruncate";
    exit(1);
  }
  
  
  void *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  Bank *bank = new (shm_ptr) Bank(n);

  sem_post(sem);

  munmap(shm_ptr, shm_size);
  close(shm_fd);
  sem_close(sem);
}
