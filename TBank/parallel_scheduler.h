#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include "includeAll.h"

using namespace std;

class parallel_scheduler 
{
private:
    vector<thread> threads;
    queue<pair<function<void(void*)>, void*>> taskQueue;
    mutex queueMutex;
    condition_variable condition;
    atomic<bool> stop;

    void workerThread();

public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(function<void(void*)> func, void* arg);
};

#endif