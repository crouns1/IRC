#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/select.h>
#include "Client.hpp"
#include "tools.hpp"
#include "CommandHandler.hpp"
#include "channel.hpp"


class Server {
private:
    int m_port;
    std::string m_serverPassword;
    int m_serverFd;
    std::map<std::string, Channel*> m_channels;
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

     // this look in the map through the name of the channel.
     // if the channel is found it return a pointer to it, if not, it return NULL.
    Channel* getChannel(const std::string& name);
    // this create a channel, and send back the pointer of it. add the new channel created in the map. 
    // if the creation fail, return NULL. 
    Channel* createChannel(const std::string& name);
    // remove the channel ( remove it from the map)
    void removeChannel(const std::string& name);
    
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