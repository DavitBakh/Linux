#include "includeAll.h"
#include "bank.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "Argument count error" << std::endl;
    return -1;
  }

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


  munmap(shm_ptr, shm_size);
  close(shm_fd);
}
