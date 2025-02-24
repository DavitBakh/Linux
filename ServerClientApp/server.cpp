#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
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

  while(true) {
      int client_socket;
      struct sockaddr_in client_address;
      unsigned int client_addr_len = sizeof(client_address);

      // Accept incoming connection
      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
          perror("accept failed");
          exit(errno);
      }

      std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

      char buffer[1001];
      // Receive message from client
      while (true)
      {        
        int rs = recv(client_socket, buffer, 1000, 0);
        if (rs == -1) 
        {
            perror("client socket connection error");
            close(client_socket);
            continue;
        }

        if(rs == 0) 
        {
            std::cout << "Client disconnected\n";
            close(client_socket);
            break;
        }
        
        if (rs > 0)
        {
            std::cout << "Got message:\n";
            buffer[rs] = '\0';
            std::cout << buffer << "\n";
        }
    }
  }
  
  // close
  close(server_socket);
  return 0;
}
