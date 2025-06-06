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

pthread_mutex_t coutMutex = PTHREAD_MUTEX_INITIALIZER;
void SafeCout(std::string message)
{
    pthread_mutex_lock(&coutMutex);
    std::cout << message;
    pthread_mutex_unlock(&coutMutex);
}


struct Client
{
    int socket;
    sockaddr_in *address;
    std::string name;

    Client() : socket(0), address(nullptr), name("Unknown") {}
    Client(int socket, sockaddr_in *address, std::string name = "Unknown") : socket(socket), address(address), name(name) {}
};

std::vector<Client *> clients;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void SendToAll(std::string message, Client *currClient)
{
    pthread_mutex_lock(&mutex);
    for (Client *client : clients)
    {
        if (client == nullptr || client->socket == currClient->socket)
            continue;

        std::cout << "Sending message from " << currClient->name << ": " << message << "\n";
        message = currClient->name + ": " + message;
        send(client->socket, message.c_str(), message.size(), 0);
    }
    pthread_mutex_unlock(&mutex);
}

void SetClientName(Client *client)
{
    if (send(client->socket, "Enter your name: ", 18, 0) < 0)
    {
        SafeCout("send failed");
        close(client->socket);
        exit(errno);
    }

    char buffer[1001];
    int rs = recv(client->socket, buffer, 1000, 0);
    if (rs < 0)
    {
        SafeCout("client socket connection error");
        close(client->socket);
        exit(errno);
    }

    if (rs == 0)
    {
        SafeCout("Client disconnected\n");
        close(client->socket);
        exit(errno);
    }

    buffer[rs] = '\0';
    client->name = std::string{buffer};
}

void RemoveClientFromList(Client *client)
{
    pthread_mutex_lock(&mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (*it == client)
        {
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *clientHandler(void *arg)
{
    char buffer[1001];
    Client *client = (Client *)arg;

    SetClientName(client);
    SafeCout("Client: " + client->name + " was connected\n");

    // Receive message from client
    while (true)
    {
        int rs = recv(client->socket, buffer, 1000, 0);
        if (rs == -1)
        {
            SafeCout("client socket connection error");
            close(client->socket);
            continue;
        }

        if (rs == 0)
        {
            SafeCout("Client: " + client->name + " disconnected\n");
            RemoveClientFromList(client);
            break;
        }

        if (rs > 0)
        {
            buffer[rs] = '\0';
            SafeCout("Got message from " + client->name + ": " + std::string{buffer} + "\n");

            if (strcmp(buffer, "/list") == 0)
            {
                std::string list = "Clients: ";
                for (Client *c : clients)
                    list += c->name + ", ";

                send(client->socket, list.c_str(), list.size(), 0);
                continue;
            }
            SendToAll(std::string{buffer}, client);
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
            SafeCout("accept failed");
            exit(errno);
        }

        SafeCout("Connected client with address: " + std::string{inet_ntoa(client_address.sin_addr)} + "\n");

        Client *client = new Client();
        client->socket = client_socket;
        client->address = &client_address;

        pthread_mutex_lock(&mutex);
        clients.push_back(client);
        pthread_mutex_unlock(&mutex);

        pthread_t thread;
        if (pthread_create(&thread, NULL, clientHandler, client) < 0)
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
    pthread_mutex_destroy(&coutMutex);

    return 0;
}
