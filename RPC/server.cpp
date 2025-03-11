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
#include <cstring>
#include <vector>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void SafeCout(const char* message)
{
    pthread_mutex_lock(&mutex);
    std::cout << message;
    pthread_mutex_unlock(&mutex);
}

const char *Calculate(char buffer[], int size)
{
    const char *delim = " ";
    char *token = strtok(buffer, delim);
    
    if(token == NULL)
    {
        return "Invalid input";
    }

    char* operand = strtok(NULL, delim);
    std::vector<int> operands;
    while (operand != NULL)
    {
        operands.push_back(atoi(operand));
        operand = strtok(NULL, delim);
    }

    double res = 0;
    if(strcmp(token, "ADD") == 0)
    {
        res = operands[0];
        for(int i = 1; i < operands.size(); i++)
        {
            res += operands[i];
        }
    }
    else if(strcmp(token, "SUB") == 0)
    {
        res = operands[0];
        for(int i = 1; i < operands.size(); i++)
        {
            res -= operands[i];
        }
    }
    else if(strcmp(token, "MUL") == 0)
    {
        res = operands[0];
        for(int i = 1; i < operands.size(); i++)
        {
            res *= operands[i];
        }
    }
    else if(strcmp(token, "DIV") == 0)
    {
        res = operands[0];
        for(int i = 1; i < operands.size(); i++)
        {
            res /= operands[i];
        }
    }
    else
    {
        return "Invalid input";
    }
    
    char* result = new char[100];
    sprintf(result, "%f", res);
    return result;
}

void *clientHandler(void *arg)
{
    char buffer[1001];
    int client_socket = *((int *)arg);
    // Receive message from client
    while (true)
    {
        int rs = recv(client_socket, buffer, 1000, 0);
        if (rs == -1)
        {
            SafeCout("client socket connection error");
            close(client_socket);
            continue;
        }

        if (rs == 0)
        {
            SafeCout("Client disconnected\n");
            close(client_socket);
            break;
        }

        if (rs > 0)
        {
            buffer[rs] = '\0';
            const char *res = Calculate(buffer, rs);
            send(client_socket, res, strlen(res), 0);
        }
    }

    close(client_socket);
    pthread_exit(NULL);

    return NULL;
}

int main()
{
    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket creation error");
        exit(errno);
    }

    // create an endpoint

    // socket address
    struct sockaddr_in server_address;
    // internet protocol = AF_INET
    server_address.sin_family = AF_INET;
    // accept or any address (bind the socket to all available interfaces)
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // port
    server_address.sin_port = htons(8888);

    // Bind server_socket to server_address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(errno);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0)
    {
        perror("listen failed");
        exit(errno);
    }
    std::cout << "Waiting for connection\n";

    while (true)
    {
        int client_socket;
        struct sockaddr_in client_address;
        unsigned int client_addr_len = sizeof(client_address);

        // Accept incoming connection
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_len)) < 0)
        {
            SafeCout("accept failed");
            exit(errno);
        }

        std::string message = "Connected client with address: " + std::string(inet_ntoa(client_address.sin_addr)) + "\n";
        SafeCout(message.c_str());

        pthread_t thread;
        if (pthread_create(&thread, NULL, clientHandler, &client_socket) < 0)
        {
            SafeCout("pthread_create failed");
            close(client_socket);
            exit(errno);
        }
        pthread_detach(thread);
    }

    // close
    close(server_socket);
    pthread_mutex_destroy(&mutex);

    return 0;
}
