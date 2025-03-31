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
#include <signal.h>

using namespace std;

#define T 0
#define P 1
#define M 2
#define B 3

struct SharedData
{
    sem_t sem[4];
    pid_t smokerPid[3];
};
SharedData *sharedData;

bool ignoreSigterm = false, isSigtermReceived = false;
void SigtermHandler(int signum)
{
    if (signum == SIGTERM)
    {
        isSigtermReceived = true;

        if (ignoreSigterm)
            return;

        exit(0);
    }
}

void Smoker(int id)
{
    signal(SIGTERM, SigtermHandler);

    while (true)
    {
        sem_wait(&sharedData->sem[id]);
        ignoreSigterm = true;
        sleep(rand() % 4 + 1); // simulate smoking

        switch (id)
        {
        case T:
            cout << 'T' << endl;
            break;
        case P:
            cout << 'P' << endl;
            break;
        case M:
            cout << 'M' << endl;
            break;
        }

        sem_post(&sharedData->sem[B]);

        ignoreSigterm = false;
        if (isSigtermReceived)
            exit(0);
    }
}

void Bartender()
{
    string items;
    getline(cin, items);

    cout << "Items: " << items << endl;

    for (char item : items)
    {
        if (item == ' ')
            continue;

        sem_wait(&sharedData->sem[B]);

        sleep(rand() % 4 + 1); // searching for items
        switch (item)
        {
        case 't':
            cout << "Find t: " << endl;
            sem_post(&sharedData->sem[T]);
            break;
        case 'p':
            cout << "Find p: " << endl;
            sem_post(&sharedData->sem[P]);
            break;
        case 'm':
            cout << "Find m: " << endl;
            sem_post(&sharedData->sem[M]);
            break;
        }
    }

    cout << "Bartender finished" << endl;

    for (int i = 0; i < 3; i++)
        kill(sharedData->smokerPid[i], SIGTERM);
}

int main()
{
    srand(time(NULL));

    sharedData = (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sharedData == MAP_FAILED)
    {
        perror("mmap failed");
        exit(errno);
    }

    for (int i = 0; i < 3; i++)
        sem_init(&sharedData->sem[i], 1, 0);

    sem_init(&sharedData->sem[B], 1, 3);

    for (int i = 0; i < 3; ++i)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(errno);
        }

        if (pid == 0)
        {
            Smoker(i);
            return 0;
        }

        sharedData->smokerPid[i] = pid;
    }

    Bartender();

    for (int i = 0; i < 3; ++i)
        wait(NULL);

    for (int i = 0; i < 4; i++)
        sem_destroy(&sharedData->sem[i]);

    return 0;
}