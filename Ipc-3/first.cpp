#include "shared_array.h"
#include <semaphore.h>

int main() 
{
    int arrSize = 20;
    SharedArray arr("shArr", arrSize);

    sem_t* writeSem = sem_open("write", O_CREAT, 0666, 1);
    sem_t* readSem = sem_open("read", O_CREAT, 0666, 1);

    if (writeSem == SEM_FAILED) 
    {
        perror("sem open error!");
        exit(errno);
    }

    while (true) 
    {
        sem_wait(writeSem);

        for (int i = 0; i < arrSize; ++i)
            arr[i] = rand() % 100;

        std::cout << "First program!" << std::endl;

        sem_post(readSem);
        sleep(1);
    }

    sem_close(writeSem);
    sem_close(readSem);

    return 0;
}