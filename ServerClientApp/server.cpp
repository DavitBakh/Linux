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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void SafeCout(std::string message)
{
    pthread_mutex_lock(&mutex);
    std::cout << message;
    pthread_mutex_unlock(&mutex);
}

void* clientHandler(void *arg)
{
    char buffer[1001];
    // Receive message from client
    while (true)
    {
        int client_socket = *((int *)arg);

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
            SafeCout("Got message:\n" + std::string{buffer} + "\n");
        }
    }

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

        SafeCout("Connected client with address: " + std::string{inet_ntoa(client_address.sin_addr)} + "\n");

        pthread_t pid;
        if(pthread_create(&pid, NULL, clientHandler, &client_socket) < 0)
        {
            SafeCout("pthread_create failed");
            close(client_socket);
            exit(errno);
        }
    }

    // close
    close(server_socket);
    pthread_mutex_destroy(&mutex);

    return 0;
}
