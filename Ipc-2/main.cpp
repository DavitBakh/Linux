#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <cerrno>
#include <string>
#include <cmath>
#include <cstdlib>

int primeN(int n) 
{
    if (n == 1) 
        return 2;

    int count = 2;
    int num = 3;
    while (count != n) 
    {
        num += 2;
        bool isprime = true;
        if(num % 2 == 0)
            continue;

        for (int i = 3; i * i <= num; i += 2) 
        {
            if (num % i == 0) 
            {
                isprime = false;
                break;
            }
        }

        if (isprime)
            ++count;
    }
    return num;
}

int main() 
{
    int fd1[2];
    int fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) 
    {
        perror("Pipe error!");
        exit(errno);
    }

    pid_t pid = fork();
    if (pid == -1) 
    {
        perror("Fork error!");
        exit(errno);
    }

    if (pid == 0) 
    {
        close(fd1[1]);
        close(fd2[0]);

        while (true) 
        {
            char buffer[100];
            int bytesRead = read(fd1[0], buffer, sizeof(buffer) - 1);

            if (bytesRead == -1) 
            { 
                perror("Read error!");
                break;
            }
            if (bytesRead == 0)
                break;

            buffer[bytesRead] = '\0';

            if (std::string(buffer) == "exit") 
            {
                std::cout << "[Child] Exiting...\n";
                break;
            }

            std::cout << "[Child] Calculating " << buffer << "-th prime number...\n";
            int prime = primeN(atoi(buffer));

            std::cout << "[Child] Sending calculation result of prime(" << buffer << ")...\n";
            if (write(fd2[1], &prime, sizeof(prime)) < 0) 
            {
                perror("Write error!");
                break;
            }
        }

        close(fd1[0]);
        close(fd2[1]);
    }
    else {
        close(fd1[0]);
        close(fd2[1]);

        std::string input;
        while (true) 
        {
            std::cout << "[Parent] Please enter the number: ";
            std::getline(std::cin, input);

            if (write(fd1[1], input.c_str(), input.size() + 1) < 0) 
            {
                perror("Write error!");
                exit(errno);
            }

            if (input == "exit")
                break;

            std::cout << "[Parent] Sending " << input << " to the child process...\n";
            std::cout << "[Parent] Waiting for the response from the child process...\n";

            int prime;
            int bytesRead = read(fd2[0], &prime, sizeof(prime));
            if (bytesRead == -1) 
            { 
                perror("Read error!");
                break;
            }
            if (bytesRead == 0)
                break;

            std::cout << "[Parent] Received calculation result of prime(" << input << ") = " << prime << "...\n\n";
        }

        close(fd1[1]);
        close(fd2[0]);
    }

    return 0;
}

