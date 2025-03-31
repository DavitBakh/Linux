#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>
#include <semaphore.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;

#define B 0
#define T 1
#define P 2
#define M 3

void Smoker(int id)
{
}

void Bartender()
{
    char item;
    cin >> item;
    switch (item)
    {
    case 'B':
        break;
    case 'T':
        break;
    case 'P':

        break;
    case 'M':
        break;
    }
}

int main()
{
    int shmSize = 4 * sizeof(sem_t);
    string shmName = "/smokers" + to_string(getpid());

    int shmFd = shm_open(shmName.c_str(), O_CREAT | O_RDWR, 0666);
    if (shmFd == -1)
    {
        perror("shm_open failed");
        exit(errno);
    }

    if (ftruncate(shmFd, shmSize) == -1)
    {
        perror("ftruncate failed");
        exit(errno);
    }

    sem_t *sem = (sem_t *)mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (sem == MAP_FAILED)
    {
        perror("mmap failed");
        exit(errno);
    }

    for (int i = 0; i < 4; i++)
        sem_init(&sem[i], 1, 1);

    for (int i = 1; i <= 3; ++i)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            Smoker(i);
            return 0;
        }
    }

    Bartender();

    // for(int i = 0; i < 4; i++)
    //    sem_destroy(&sem[i]);

    return 0;
}