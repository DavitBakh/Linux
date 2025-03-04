#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // server address
    struct sockaddr_in server_address;

    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket creation error");
        exit(errno);
    }

    // fill the server ip and port address
    server_address.sin_addr.s_addr = inet_addr("146.190.62.39");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);

    // connect
    int connected = connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (connected == -1)
    {
        perror("connection failed");
        exit(errno);
    }

	int fd = open("httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
    if(fd == -1)
    {
        perror("open failed");
        exit(errno);
    }

    // send message to server
    std::string mess = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    int sent = send(server_socket, mess.c_str(), mess.size(), 0);
    if (sent == -1)
        exit(errno);


    char buffer[3001];
    int rs = 0;
    while ((rs = recv(server_socket, buffer, 3000, 0)) > 0)
    {
        buffer[rs] = '\0';
        if(write(fd, buffer, rs) == -1)
        {
            perror("write failed");
            exit(errno);
        }
    }

    if(rs == -1)
    {
        perror("recv failed");
        exit(errno);
    }  

    close(server_socket);
    close(fd);
    return 0;
}
