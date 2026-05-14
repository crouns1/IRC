#include <iostream>
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
using namespace std;

int main(int counter , char **vectors) {
    if(counter < 3) 
	    return 1;
    int port  = atoi(vectors[1]);
    char *pass = vectors[2];
    struct addrinfo hints, *res;
    struct pollfd *fds;
    int retval;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	
    getaddrinfo(NULL, vectors[1], &hints, &res);

    int server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int yes = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    bind(server_fd, res->ai_addr, res->ai_addrlen);
    listen(server_fd, 10);
    std::cout << "Waiting for client...\n";
    while (true) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        std::cout << "Client connected!\n";

        char buffer[1024];

        while (true) {
            memset(buffer, 0, sizeof(buffer));

            int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

            if (bytes <= 0 ) {
                std::cout << "Client disconnected\n";
                break;
            }

            std::cout << "Client says: " << buffer << std::endl;
        }
	close(client_fd);
    }

    return 0;
}
