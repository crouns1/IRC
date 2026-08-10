#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void Server::handleClientData(int clientFd) {
    char buffer[BUFFER_SIZE];
    std::memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytes = recv(clientFd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        cleanupClient(clientFd);
        return;
    }
    if (bytes == 0) {
        cleanupClient(clientFd);
        return;
    }

    std::string input(buffer);

    Client* client = m_clients[clientFd];
    if (!client) return;

    client->setReadBuffer(client->getReadBuffer() + input);

    if (client->getReadBuffer().size() > 8192) {
        cleanupClient(clientFd);
        return;
    }

    std::string currentBuffer = client->getReadBuffer();
    size_t pos;

    while ((pos = currentBuffer.find('\n')) != std::string::npos && !client->shouldDisconnect()) {
        std::string rawCmd = currentBuffer.substr(0, pos + 1);

        currentBuffer.erase(0, pos + 1);
        client->setReadBuffer(currentBuffer);

        Command cmd = parser(rawCmd);

        CommandHandler handler;
        handler.dispatch(this, client, cmd);

        currentBuffer = client->getReadBuffer();
    }

    if (client->shouldDisconnect()) {
        std::cout << "you are here" << std::endl;
        cleanupClient(clientFd);
    }
}

void Server::cleanupClient(int clientFd) {
    std::cout << "Client disconnected (FD: " << clientFd << ")" << std::endl;

    Client* client = NULL;
    if (m_clients.find(clientFd) != m_clients.end())
        client = m_clients[clientFd];

    if (client)
        removeClientFromChannels(client, "Connection closed");

    if (client)
        flushClientWrites(clientFd);

    FD_CLR(clientFd, &m_readFds);
    FD_CLR(clientFd, &m_writeFds);

    shutdown(clientFd, SHUT_WR);
    usleep(100000);
    close(clientFd);

    if (client) {
        delete client;
        m_clients.erase(clientFd);
    }

    if (clientFd == m_maxFd) {
        m_maxFd = m_serverFd;
        for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
            if (it->first > m_maxFd) {
                m_maxFd = it->first;
            }
        }
    }
}

bool Server::flushClientWrites(int clientFd) {
    Client* client = getClient(clientFd);
    if (!client)
        return false;

    std::string& buffer = client->getWriteBuffer();
    while (!buffer.empty()) {
        ssize_t sent = send(clientFd, buffer.c_str(), buffer.size(), MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return true;
            return false;
        }
        buffer.erase(0, static_cast<size_t>(sent));
    }

    FD_CLR(clientFd, &m_writeFds);
    return true;
}

void Server::sendResponse(int clientFd, const std::string& response) {
    if (m_clients.find(clientFd) == m_clients.end()) {
        return;
    }

    std::string fullResponse = response + "\r\n";
    send(clientFd, fullResponse.c_str(), fullResponse.length(), MSG_NOSIGNAL);
}
