#include "includeAll.h"
#include "parallel_scheduler.h"
#include "bank.h"

#define PORT 8888
#define MAX_CLIENTS 100
#define BUFFER_SIZE 4096
#define SERVER_TIMEOUT -1 // 10000 == 10 seconds
#define POOLSIZE 10

Bank *bank;

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

void SafeCout(std::string message)
{
    pthread_mutex_lock(&_mutex);
    std::cout << message;
    pthread_mutex_unlock(&_mutex);
}

struct Client
{
    int socket;
    char *buffer;
    int size;

    Client(int socket, char *buffer, int size) : socket(socket), buffer(buffer), size(size) {}
};

string DoCommand(vector<int> &operands)
{
    string res;

    switch (operands[0])
    {
    case -1:
        return bank->GetCommandsList();
    case 1:
        return to_string(bank->GetBalance(operands[1]));
    case 2:
        return to_string(bank->GetMinBalance(operands[1]));
    case 3:
        return to_string(bank->GetMaxBalance(operands[1]));
    case 4:
        if (bank->isFrozen(operands[1]))
        {
            cout << "Account: " << operands[1] << " is already frozen" << endl;
            return "Account is already frozen";
        }

        return bank->froze_defroze(operands[1]) ? bank->successMessage : bank->errorMessage;
    case 5:
        if (!bank->isFrozen(operands[1]))
        {
            cout << "Account: " << operands[1] << " is already defrozen" << endl;
            return "Account is already defrozen";
        }
        return bank->froze_defroze(operands[1]) ? bank->successMessage : bank->errorMessage;

    case 6:
        return bank->transfer(operands[1], operands[2], operands[3]) ? bank->successMessage : bank->errorMessage;
    case 7:
        return bank->writeOffFromAll(operands[1]) ? bank->successMessage : bank->errorMessage;
    case 8:
        return bank->creditToAll(operands[1]) ? bank->successMessage : bank->errorMessage;
    case 9:
        return bank->setMinBalance(operands[1], operands[2]) ? bank->successMessage : bank->errorMessage;
    case 10:
        return bank->setMaxBalance(operands[1], operands[2]) ? bank->successMessage : bank->errorMessage;
    case 11:
        return bank->GetAll();
    default:
        return "Unknown command";
    }
}

void *clientHandler(void *arg)
{
    Client *client = (Client *)arg;

    std::vector<int> operands;
    char *token = std::strtok(client->buffer, " ");
    while (token != nullptr)
    {
        operands.push_back(atoi(token));
        token = std::strtok(nullptr, " ");
    }

    string res = DoCommand(operands);
    std::cout << "Sending response: " << res << std::endl;

    int html_fd = open("index.html", O_RDONLY);
    
    char body[] = "Hello from server!";
    char response[1024];

    int len = snprintf(response, sizeof(response),
                       "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/plain\r\n"
                       "Content-Length: %zu\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       strlen(body), body);

    // Then send the response:
    send(client->socket, response, len, 0);
    return NULL;
}

int main(int argc, char *argv[])
{
    char response[16384] = "HTTP/1.1 200 OK\r\n\n";
    int html_fd = open("index.html", O_RDONLY);
    char html[16384];
    int bites = read(html_fd, html, 16384);
    html[bites] = '\0';

    strcat(response, html);

    bool isHttp = false;
    if (argc == 2)
    {
        isHttp = true;
        std::cout << "Server mode http" << std::endl;
    }

    int shm_fd = shm_open("/TBank", O_RDWR, 0666);
    struct stat sb;
    if (fstat(shm_fd, &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    int shm_size = sb.st_size;
    void *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    bank = (Bank *)shm_ptr;

    parallel_scheduler scheduler(POOLSIZE);

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
        int ret = poll(fds, MAX_CLIENTS + 1, SERVER_TIMEOUT);
        if (ret == -1)
        {
            perror("poll failed");
            exit(errno);
        }

        if (ret == 0)
        {
            std::cout << "Server timeout" << std::endl;
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            ret--;

            int client_socket;
            struct sockaddr_in client_address;
            unsigned int client_addr_len = sizeof(client_address);

            if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_addr_len)) < 0)
            {
                perror("accept failed");
                exit(errno);
            }

            if (isHttp)
            {
                send(client_socket, response, sizeof(response), 0);
            }
            else
            {
                std::string welcome_message = bank->GetCommandsList();
                send(client_socket, welcome_message.c_str(), welcome_message.size(), 0);
            }

            std::cout << "Connected client with address: " << std::string{inet_ntoa(client_address.sin_addr)} << std::endl;

            int i = 1;
            for (; i <= MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    if (i > MAX_CLIENTS)
                    {
                        std::cout << "No more clients can be connected" << std::endl;
                        close(client_socket);
                        break;
                    }

                    fds[i].fd = client_socket;
                    fds[i].events = POLLIN;
                    break;
                }
            }
        }

        for (int i = 1; i <= MAX_CLIENTS && ret; i++)
        {
            if (fds[i].fd != -1 && fds[i].revents & POLLIN)
            {
                ret--;

                char buffer[BUFFER_SIZE + 1];
                int rs = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);

                if (rs == -1)
                {
                    perror("client socket connection error");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }

                if (rs == 0)
                {
                    perror("Client disconnected\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }

                if (rs > 0)
                {
                    buffer[rs] = '\0';

                    if (isHttp)
                    {
                        const char *lastNewline = strrchr(buffer, '\n');
                        int index = 0;
                        while (lastNewline[index + 1] != '\0')
                        {
                            buffer[index] = lastNewline[index + 1];
                            index++;
                        }
                        buffer[index] = '\0';

                        if (index == 0)
                        {
                            close(fds[i].fd);
                            fds[i].fd = -1;
                            continue;
                        }
                    }
                    Client client(fds[i].fd, buffer, rs);

                    std::cout << "Received command: " << buffer << std::endl;
                    scheduler.run(clientHandler, &client);
                }
            }
        }
    }

    close(server_socket);
    munmap(shm_ptr, shm_size);
    close(shm_fd);
    return 0;
}
