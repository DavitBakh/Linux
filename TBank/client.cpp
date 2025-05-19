#include "includeAll.h"

void* reciver(void* args)
{
    int server_socket = *(int*)args;

    char buffer[3001];
    int rs;
    while((rs = recv(server_socket, buffer, 3000, 0)) > 0)
    {
        buffer[rs] = '\0';
        std::cout << buffer << std::endl;
    }

    return nullptr;
}

int main()
{
    struct sockaddr_in server_address;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket creation error");
        exit(errno);
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    int connected = connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connected == -1)
    {
        perror("connection failed");
        exit(errno);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, reciver, &server_socket);
    pthread_detach(thread);

    std::string mess;
    while (true)
    {
        getline(std::cin, mess);

        if(mess == "/exit" || mess == "0")
        {
            close(server_socket);
            return 0;
        }
        
        int sent = send(server_socket, mess.c_str(), mess.size(), 0);
        if (sent == -1)
            exit(errno);
    }

    close(server_socket);
    return 0;
}
