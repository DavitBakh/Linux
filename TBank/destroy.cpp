#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int main()
{
  sem_unlink("/TBankSem");
  shm_unlink("/TransparentBank");
}
