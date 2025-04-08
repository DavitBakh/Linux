#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  shm_unlink("/TransparentBank");
}
