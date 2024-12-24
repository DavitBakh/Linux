#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <chrono>

using namespace std;

struct ThreadData
{
    int* arr;
    int size;
    int start;
    int end;

    ThreadData(int* arr, int size, int start, int end)
    {
        this->arr = arr;
        this->size = size;
        this->start = start;
        this->end = end;
    }
};

void* CalculateSum(void* arg)
{
    ThreadData* threadData = (ThreadData*)arg;

    unsigned long long sum = 0;
    for (int i = threadData->start; i < threadData->end; i++)
        sum += threadData->arr[i];

    return (void*)sum;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "invalid arguments count" << endl;
        return 1;
    }

    size_t n = stoull(argv[1]);
    int* arr = new int[n];

    srand(time(0));
    for (size_t i = 0; i < n; i++)
        arr[i] = rand() % 100;
        
    auto start = chrono::high_resolution_clock::now();
    unsigned long long sumNoThread = 0;
    for (size_t i = 0; i < n; i++)
        sumNoThread += arr[i];

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time spent without threads: " << duration.count() << " seconds" << std::endl; 

    size_t m = stoull(argv[2]);

    start = chrono::high_resolution_clock::now();
    pthread_t* threads = new pthread_t[m];
    ThreadData** threadData = new ThreadData*[m];

    int partSize = n / m;
    for (size_t i = 0; i < m; i++)
    {
        int start = i * partSize;
        int end = i + 1 == m ? n : start + partSize;
        threadData[i] = new ThreadData(arr, n, start, end);
        pthread_create(&threads[i], NULL, CalculateSum, threadData[i]);
    }

    unsigned long long sumThread = 0;
    for (size_t i = 0; i < m; i++)
    {
        void* result;
        pthread_join(threads[i], &result);
        sumThread += (unsigned long long)result;
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Time spent with " << m << " threads: " << duration.count() << " seconds" << std::endl;
    return 0;
}