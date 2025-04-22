#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int main()
{
  shm_unlink("/TBank");
}
