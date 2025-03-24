#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <mutex>
#include <string>
#include <poll.h>
#include <vector>
//#include "parallel_scheduler.h"

#define PORT 8888
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define SERVER_TIMEOUT 10000 // 10 seconds
#define POOLSIZE 10

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

void SafeCout(std::string message)
{
    pthread_mutex_lock(&_mutex);
    std::cout << message;
    pthread_mutex_unlock(&_mutex);
}

const char *Calculate(char buffer[], int size)
{
    const char *delim = " ";
    char *token = strtok(buffer, delim);

    if (token == NULL)
    {
        return "Invalid input";
    }

    char *operand = strtok(NULL, delim);
    std::vector<int> operands;
    while (operand != NULL)
    {
        operands.push_back(atoi(operand));
        operand = strtok(NULL, delim);
    }

    double res = 0;
    if (strcmp(token, "ADD") == 0)
    {
        res = operands[0];
        for (size_t i = 1; i < operands.size(); i++)
        {
            res += operands[i];
        }
    }
    else if (strcmp(token, "SUB") == 0)
    {
        res = operands[0];
        for (size_t i = 1; i < operands.size(); i++)
        {
            res -= operands[i];
        }
    }
    else if (strcmp(token, "MUL") == 0)
    {
        res = operands[0];
        for (size_t i = 1; i < operands.size(); i++)
        {
            res *= operands[i];
        }
    }
    else if (strcmp(token, "DIV") == 0)
    {
        res = operands[0];
        for (size_t i = 1; i < operands.size(); i++)
        {
            res /= operands[i];
        }
    }
    else
    {
        return "Invalid input";
    }

    char *result = new char[100];
    sprintf(result, "%f", res);
    return result;
}

void *clientHandler(void *arg)
{

    std::cout << "clientHandler\n";
    char buffer[BUFFER_SIZE + 1];
    int* client_socket = (int *)arg;

    int rs = recv(*client_socket, buffer, BUFFER_SIZE, 0);
    if (rs == -1)
    {
        SafeCout("client socket connection error");
        close(*client_socket);
    }

    if (rs == 0)
    {
        SafeCout("Client disconnected\n");
        close(*client_socket);
        *client_socket = -1;
    }

    if (rs > 0)
    {
        buffer[rs] = '\0';
        const char *res = Calculate(buffer, rs);
        send(*client_socket, res, strlen(res), 0);
    }

    return NULL;
}

int main()
{
    //parallel_scheduler scheduler(POOLSIZE);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket creation error");
        exit(errno);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
    {
        perror("setsockopt failed");
        close(server_socket);
        exit(errno);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(errno);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(errno);
    }
    std::cout << "Waiting for connection\n";

    struct pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = server_socket;
    fds[0].events = POLLIN; 

    for (int i = 1; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;

    while (true)
    {
        std::cout << "Poll\n";
        int ret = poll(fds, MAX_CLIENTS + 1, SERVER_TIMEOUT);
        if (ret == -1)
        {
            perror("poll failed");
            exit(errno);
        }

        if(ret == 0)
        {
            std::cout << "Server timeout" << std::endl;
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            int client_socket;
            struct sockaddr_in client_address;
            unsigned int client_addr_len = sizeof(client_address);

            if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_len)) < 0)
            {
                SafeCout("accept failed");
                exit(errno);
            }

            SafeCout("Connected client with address: " + std::string{inet_ntoa(client_address.sin_addr)} + "\n");

            for (int i = 1; i <= MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    fds[i].fd = client_socket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        std::cout << "TEMP\n";
        for (int i = 1; i <= MAX_CLIENTS; i++)
        {
            if (fds[i].fd != -1 && fds[i].revents & POLLIN)
            {
                //scheduler.run(clientHandler, &fds[i].fd);
                pthread_t thread;
                pthread_create(&thread, NULL, clientHandler, &fds[i].fd);
                pthread_detach(thread);
                sleep(100);
            }

            std::cout << "FOR\n";
        }

        std::cout <<"end\n";
    }

    // close
    close(server_socket);
    pthread_mutex_destroy(&_mutex);

    return 0;
}
