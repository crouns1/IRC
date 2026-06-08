#include "CommandHandler.hpp"
#include "tools.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void fillClient(Client *client) // this should be partially with the server. 
{
  t_ClientData cltdata;
  cltdata.m_nickname = "NickName";
  cltdata.m_username = "UserName";
  cltdata.m_realname = "RealName";
  cltdata.m_hostname = "1.1.1.1.1";
  client->setData(cltdata);
}
#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "bind() failed" << std::endl;
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, 5) < 0)
    {
        std::cerr << "listen() failed" << std::endl;
        close(serverFd);
        return 1;
    }

    std::cout << "Listening on port " << PORT << std::endl;

    while (true)
    {
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd < 0)
            continue;

        Client client(clientFd, "127.0.0.1");
        fillClient(&client);

        char buffer[BUFFER_SIZE];

        while (true)
        {
            ssize_t bytes = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

            if (bytes <= 0)
                break;

            buffer[bytes] = '\0';

            std::string input(buffer);

            std::cout << "Received: " << input << std::endl;

            // Parse and dispatch using your existing code
            Command cmd = parser(input);

            CommandHandler handler;
            handler.dispatch(&client, cmd);

            // Echo back
            send(clientFd, buffer, bytes, 0);
        }

        close(clientFd);
    }

    close(serverFd);
    return 0;
}