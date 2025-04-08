#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include "bank.h"

int main(int argc, char **argv)
{
  shm_unlink("/TransparentBank");
}
