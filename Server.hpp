#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/select.h>
#include "Client.hpp"
#include "tools.hpp"
#include "CommandHandler.hpp"

class Server {
private:
    int m_port;
    std::string m_serverPassword;
    int m_serverFd;
    
    // Client management
    std::map<int, Client*> m_clients;
    
    // FD sets for select()
    fd_set m_readFds;
    fd_set m_writeFds;
    int m_maxFd;
    
    // Initialize server socket
    bool initSocket();
    
    // Handle new connections
    void acceptNewClient();
    
    // Handle client data
    void handleClientData(int clientFd);
    
    // Cleanup disconnected clients
    void cleanupClient(int clientFd);
    
    // Send IRC response to client
    void sendResponse(int clientFd, const std::string& response);
    
public:
    Server(int port, const std::string& password);
    ~Server();
    
    // Main server loop
    void run();
    
    // Validate client password
    bool validatePassword(const std::string& password);
    bool isNicknameInUse(const std::string& nickname);
    // Get client by fd
    Client* getClient(int fd);
    void printList();
    Client* getClientByNickname(const std::string& nickname);
    
    // Remove client
    void removeClient(int fd);
};

#endif