#include "parallel_scheduler.h"

using namespace std;

parallel_scheduler::parallel_scheduler(size_t capacity) : stop(false) 
{
    for (size_t i = 0; i < capacity; ++i) 
        threads.emplace_back(&parallel_scheduler::workerThread, this);
}

parallel_scheduler::~parallel_scheduler() 
{
    stop.store(true);
    condition.notify_all();
    for (thread& thread : threads) 
    {
        if (thread.joinable())
            thread.join();
    }
}

void parallel_scheduler::run(function<void(void*)> func, void* arg) 
{
    {
        lock_guard<mutex> lock(queueMutex);
        taskQueue.emplace(func, arg);
    }
    condition.notify_one();
}

void parallel_scheduler::workerThread() 
{
    while (!stop) 
    {
        function<void(void*)> task;
        void* arg;

        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return stop || !taskQueue.empty(); });

            if (stop && taskQueue.empty()) return;

            task = taskQueue.front().first;
            arg = taskQueue.front().second;
            taskQueue.pop();
        }

        task(arg);
    }
}