#include "Server.hpp"
#include <iostream>

bool Server::validatePassword(const std::string& password) {
    return password == m_serverPassword;
}

Client* Server::getClient(int fd) {
    if (m_clients.find(fd) != m_clients.end()) {
        return m_clients[fd];
    }
    return NULL;
}

void Server::removeClient(int fd) {
    cleanupClient(fd);
}

bool Server::isNicknameInUse(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (it->second->getData().m_nickname == nickname) {
            return true;
        }
    }
    return false;
}

bool Server::isNicknameInUse(const std::string& nickname, Client* exclude) {
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (it->second != exclude && it->second->getData().m_nickname == nickname) {
            return true;
        }
    }
    return false;
}

Client* Server::getClientByNickname(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (it->second->getData().m_nickname == nickname) {
            return it->second;
        }
    }
    return NULL;
}
