#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>
#include <unistd.h>

using namespace std;

void exampleTask(void* arg) 
{
    int taskId = *static_cast<int*>(arg);
    cout << "Task " << taskId << " is running on thread " << this_thread::get_id() << endl;
    sleep(1);
    cout << "Task " << taskId << " is completed." << endl;
}

int main(int argc, char* argv[]) 
{
    constexpr size_t poolSize = 5;
    parallel_scheduler scheduler(poolSize);

    constexpr int tasksNum = 10;
    int tasksIds[tasksNum];

    for (int i = 0; i < tasksNum; ++i) 
    {
        tasksIds[i] = i + 1;
        scheduler.run(exampleTask, &tasksIds[i]);
    }

    sleep(10);
    return 0;
}