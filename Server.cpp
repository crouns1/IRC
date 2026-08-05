#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

Server::Server(int port, const std::string& password) 
    : m_port(port), m_serverPassword(password), m_serverName("irc.local"), m_serverFd(-1), m_maxFd(0) {
    FD_ZERO(&m_readFds);
    FD_ZERO(&m_writeFds);
}

Server::~Server() {
    // kaniterateiw bach ndiro Cleanup l all clients
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
// hna kandiro establish l connection dyalna
bool Server::initSocket() {
    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverFd < 0) {
        std::cerr << "socket() failed" << std::endl;
        return false;
    }
    
    // hna its important bach ndiro set socket option bach  ndiro reuse address
    int opt = 1;
    if (setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed" << std::endl;
        close(m_serverFd);
        return false;
    }
    
    sockaddr_in addr;
    // ..
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
    
    
    // add server socket to read set
    FD_SET(m_serverFd, &m_readFds);
    m_maxFd = m_serverFd;
    
    std::cout << "IRC Server listening on port " << m_port << std::endl;
    return true;
}

void Server::run() {
    if (!initSocket()) {
        return;
    }
    
    while (true) {
        // register clients with queued output in the write set so a slow
        // reader never blocks the whole server
        for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
            if (!it->second->getWriteBuffer().empty())
                FD_SET(it->first, &m_writeFds);
        }

        fd_set readFds = m_readFds;
        fd_set writeFds = m_writeFds;
        
        // hna iam using select() bach  nmonitoriw multiple fd's
        int activity = select(m_maxFd + 1, &readFds, &writeFds, NULL, NULL);
        
        if (activity < 0) {
            std::cerr << "select() error" << std::endl;
            continue;
        }
        
        // Check for new connections
        if (FD_ISSET(m_serverFd, &readFds)) {
            acceptNewClient();
        }
        
        // Check for data from existing clients
        // Create a copy of client FDs to avoid iterator invalidation if clients disconnect
        std::vector<int> clientFds;
        for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
            clientFds.push_back(it->first);
        }
        
        for (size_t i = 0; i < clientFds.size(); ++i) {
            int clientFd = clientFds[i];
            // Check if client still exists (might have been removed during iteration)
            if (m_clients.find(clientFd) != m_clients.end() && FD_ISSET(clientFd, &readFds)) {
                handleClientData(clientFd);
            }
        }
        
        // flush pending output for writable clients
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
    
    // non-blocking client socket so no single client can stall the server
    int flags = fcntl(clientFd, F_GETFL, 0);
    fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
    
    // Get client IP address
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, INET_ADDRSTRLEN);
    
    // Create new client
    Client* client = new Client(clientFd, ipStr);
    m_clients[clientFd] = client;
    
    // Add to select set
    FD_SET(clientFd, &m_readFds);
    if (clientFd > m_maxFd) {
        m_maxFd = clientFd;
    }
    
    std::cout << "New client connected from " << ipStr << " (FD: " << clientFd << ")" << std::endl;
    
    // Send welcome message
    sendResponse(clientFd, "Welcome to the IRC Server");
}

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
    
    // guard against unbounded buffer growth on a flood of partial lines
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
        cleanupClient(clientFd);
    }
}

void Server::cleanupClient(int clientFd) {
    std::cout << "Client disconnected (FD: " << clientFd << ")" << std::endl;
    
    Client* client = NULL;
    if (m_clients.find(clientFd) != m_clients.end())
        client = m_clients[clientFd];
    
    // remove from every channel it joined 
    if (client)
        removeClientFromChannels(client, "Connection closed");
    
    if (client)
        flushClientWrites(clientFd);
    
    // Remove from select sets
    FD_CLR(clientFd, &m_readFds);
    FD_CLR(clientFd, &m_writeFds);
    
    // Close socket
    close(clientFd);
    
    // Delete client object
    if (client) {
        delete client;
        m_clients.erase(clientFd);
    }
    
    // Update max fd if needed
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
                return true; // try again later, fd stays in the write set
            return false;    // hard error, drop the client
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
// here where i check validation of pass against server 
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

void Server::printList()
{
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        it->second->sendMessage(it->second->getData().m_username);
    }
}

Client* Server::getClientByNickname(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (it->second->getData().m_nickname == nickname) {
            return it->second;
        }
    }
    return NULL;
}

const std::string& Server::getServerName() const {
    return m_serverName;
}

void Server::removeChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = m_channels.find(name);
    if (it != m_channels.end()) {
        delete it->second;
        m_channels.erase(it);
    }
}

void Server::removeClientFromChannels(Client* client, const std::string& reason) {
    t_ClientData& data = client->getData();
    std::string nick = data.m_nickname.empty() ? "*" : data.m_nickname;
    std::string host = data.m_hostname.empty() ? "127.0.0.1" : data.m_hostname;
    std::string quitMsg = ":" + nick + "!" + data.m_username + "@" + host + " QUIT :" + reason;

    std::vector<std::string> emptyChannels;

    for (std::map<std::string, Channel*>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        Channel* channel = it->second;
        if (channel->hasClient(client)) {
            channel->broadcast(quitMsg, client);
            channel->removeClient(client);
            channel->removeOperator(client);
            if (channel->isEmpty())
                emptyChannels.push_back(it->first);
        }
    }

    for (size_t i = 0; i < emptyChannels.size(); ++i)
        removeChannel(emptyChannels[i]);
}

void Server::broadcastToChannelsOf(Client* client, const std::string& message) {
    for (std::map<std::string, Channel*>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        if (it->second->hasClient(client))
            it->second->broadcast(message, NULL);
    }
}
