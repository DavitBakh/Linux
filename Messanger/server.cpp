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
#include <vector>

struct Client
{
    int socket;
    sockaddr_in* address;
};

std::vector<Client*> clients;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void SendToAll(std::string message, int currSocket)
{
    pthread_mutex_lock(&mutex);
    for (Client* client : clients)
    {
        if(client == nullptr || client->socket == currSocket)
            continue;

        std::cout << "Sending message:\n" << message << "\n";
        send(client->socket, message.c_str(), message.size(), 0);
    }
    pthread_mutex_unlock(&mutex);
}

void* clientHandler(void *arg)
{
    char buffer[1001];
    Client* client = (Client*)arg;
    // Receive message from client
    while (true)
    {
        int rs = recv(client->socket, buffer, 1000, 0);
        if (rs == -1)
        {
            printf("client socket connection error");
            close(client->socket);
            continue;
        }

        if (rs == 0)
        {
            printf("Client disconnected\n");
            close(client->socket);
            break;
        }

        if (rs > 0)
        {
            std::cout << "Got message:\n" << buffer << "\n";
            buffer[rs] = '\0';
            SendToAll(std::string{buffer} + "\n", client->socket);
        }
    }

    close(client->socket);
    delete client;
    client = nullptr;
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
            perror("accept failed");
            exit(errno);
        }

        std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

        Client* client = new Client();
        client->socket = client_socket;
        client->address = &client_address;

        pthread_mutex_lock(&mutex);
        clients.push_back(client);
        pthread_mutex_unlock(&mutex);

        pthread_t thread;
        if(pthread_create(&thread, NULL, clientHandler, client) < 0)
        {
            perror("pthread_create failed");
            close(client_socket);
            exit(errno);
        }
        pthread_detach(thread);
    }

    // close
    close(server_socket);

    return 0;
}
