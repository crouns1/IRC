
#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <cerrno>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

int g_sd  = 0;

void siginthandler(int sig) {
    (void)sig;
    g_sd = 1;
}

bool Server::initSocket() {
    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverFd < 0) {
        std::cerr << "socket() failed" << std::endl;
        return false;
    }

    int opt = 1;
    if (setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed" << std::endl;
        close(m_serverFd);
        return false;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(m_port);

    if (bind(m_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed" << std::endl;
        close(m_serverFd);
        return false;
    }

    if (listen(m_serverFd, 5) < 0) {
        std::cerr << "listen() failed" << std::endl;
        close(m_serverFd);
        return false;
    }

    FD_SET(m_serverFd, &m_readFds);
    m_maxFd = m_serverFd;

    std::cout << "IRC Server listening on port " << m_port << std::endl;
    return true;
}

void Server::run() {
    signal(SIGINT , siginthandler);
    if (!initSocket()) {
        return;
    }
    while (!g_sd) {
        for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
            if (!it->second->getWriteBuffer().empty())
                FD_SET(it->first, &m_writeFds);
        }

        fd_set readFds = m_readFds;
        fd_set writeFds = m_writeFds;

        int activity = select(m_maxFd + 1, &readFds, &writeFds, NULL, NULL);

        if (activity < 0) {
            if(errno == EINTR) {
                break; 
            }
            std::cerr << "select() error" << std::endl;
            continue;
        }

        if (FD_ISSET(m_serverFd, &readFds)) {
            acceptNewClient();
        }

        std::vector<int> clientFds;
        for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
            clientFds.push_back(it->first);
        }

        for (size_t i = 0; i < clientFds.size(); ++i) {
            int clientFd = clientFds[i];
            if (m_clients.find(clientFd) != m_clients.end() && FD_ISSET(clientFd, &readFds)) {
                handleClientData(clientFd);
            }
        }

        for (size_t i = 0; i < clientFds.size(); ++i) {
            int clientFd = clientFds[i];
            if (m_clients.find(clientFd) != m_clients.end() && FD_ISSET(clientFd, &writeFds)) {
                if (!flushClientWrites(clientFd)) {
                    cleanupClient(clientFd);
                }
            }
        }
    }
}

void Server::acceptNewClient() {
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(m_serverFd, (sockaddr*)&clientAddr, &addrLen);
    if (clientFd < 0) {
        std::cerr << "accept() failed" << std::endl;
        return;
    }

    if (m_clients.size() >= MAX_CLIENTS) {
        const char* msg = "ERROR :Server is full\r\n";
        send(clientFd, msg, std::strlen(msg), MSG_NOSIGNAL);
        close(clientFd);
        return;
    }
    int flags = fcntl(clientFd, F_GETFL, 0);
    if (flags >= 0)
        fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
    else
        fcntl(clientFd, F_SETFL, O_NONBLOCK);

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, INET_ADDRSTRLEN);

    Client* client = new Client(clientFd, ipStr);
    m_clients[clientFd] = client;

    FD_SET(clientFd, &m_readFds);
    if (clientFd > m_maxFd) {
        m_maxFd = clientFd;
    }

    std::cout << "New client connected from " << ipStr << " (FD: " << clientFd << ")" << std::endl;

    sendResponse(clientFd, "Welcome to the IRC Server");
}
