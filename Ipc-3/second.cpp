#include "shared_array.h"
#include <semaphore.h>

int main() 
{
    int arrSize = 20;
    SharedArray arr("shArr", arrSize);

    sem_t* writeSem = sem_open("write", 0);
    sem_t* readSem = sem_open("read", 0);

    if (writeSem == SEM_FAILED) 
    {

        perror("sem open error!");
        exit(errno);
    }

    while (true) 
    {
        sem_wait(readSem);

        std::cout << "Second program!";
        for (int i = 0; i < arrSize; ++i)
            std::cout << " " << arr[i];
        std::cout << std::endl;

        sem_post(writeSem);
        sleep(1);
    }

    sem_close(writeSem);
    sem_close(readSem);

    return 0;
}