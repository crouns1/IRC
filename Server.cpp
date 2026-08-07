#include "Server.hpp"
#include <iostream>
#include <unistd.h>

Server::Server(int port, const std::string& password)
    : m_port(port), m_serverPassword(password), m_serverName("irc.local"), m_serverFd(-1), m_maxFd(0) {
    FD_ZERO(&m_readFds);
    FD_ZERO(&m_writeFds);
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        close(it->first);
        delete it->second;
    }
    m_clients.clear();

    for (std::map<std::string, Channel*>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        delete it->second;
    }
    m_channels.clear();

    if (m_serverFd >= 0) {
        close(m_serverFd);
    }
}

const std::string& Server::getServerName() const {
    return m_serverName;
}
