#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>
#include <semaphore.h>
#include <vector>

using namespace std;

#define COUNT 5
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define WHITE   "\x1B[37m"
string color[5] = {RED, GREEN, YELLOW, BLUE, WHITE}; 

sem_t stick[COUNT];


void philosopher(int n)
{

    int l = n, r = (n+1) % 5;
    if(n == 0)
        swap(l, r);

    while (true)
    {
        std::cout << color[n] << n << ": is thinking" << RESET << std::endl; 
        sleep(rand() % 10); 
        sem_wait(&stick[l]);
        sem_wait(&stick[r]);

        std::cout << color[n] << n << ": is eating" << RESET << std::endl; 
        sleep(rand() % 10);
        sem_post(&stick[l]);
        sem_post(&stick[r]);
    }
    
}

int main()
{
    srand(time(0));

    for (int i = 0; i < COUNT; i++)
        sem_init(&stick[i], 1,1);

    vector<pid_t> pids;
    
    for (int i = 0; i < COUNT; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            philosopher(i);
            exit(0);
        }
        else if(pid > 0)
            pids.push_back(pid);
        else if(pid == -1)
        {
            std::cerr << "Error: Fork failed" << std::endl;

            for(pid_t p : pids)
                kill(p, SIGKILL); 

            exit(-1);
        }
    }

    
    for (int i = 0; i < COUNT; i++)
        wait(NULL);

    for (int i = 0; i < COUNT; i++)
        sem_destroy(&stick[i]);
    
}