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
#include <fcntl.h>

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
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9999);

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

    int html_fd = open("index.html", O_RDONLY);
    char html[4096];
    int bites = read(html_fd, html, 4096);
    html[bites] = '\0';

    char response[4096] = "HTTP/1.1 200 OK\r\n\n";
    strcat(response, html);

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
        send(client_socket, response, sizeof(response), 0);
        close(client_socket);
        
    }

    // close
    close(server_socket);

    return 0;
}
