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
    std::string m_serverName;
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
    
    // Flush a client's queued output; returns false on a hard socket error
    bool flushClientWrites(int clientFd);
    
     // Send IRC response to client
    void sendResponse(int clientFd, const std::string& response);

public:
    Server(int port, const std::string& password);
    ~Server();

    // remove the channel ( remove it from the map)
    void removeChannel(const std::string& name);
    
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
    const std::string& getServerName() const;

    // remove a client from every channel it has joined, broadcasting its quit.
    // empty channels are deleted.
    void removeClientFromChannels(Client* client, const std::string& reason);

    // broadcast a message to every channel the client has joined
    // (including the client itself)
    void broadcastToChannelsOf(Client* client, const std::string& message);

     // this look in the map through the name of the channel.
     // if the channel is found it return a pointer to it, if not, it return NULL.
    Channel* getChannel(const std::string& name)
    {
      std::map<std::string, Channel*>::iterator it = m_channels.find(name);
      if (it != m_channels.end())
        return it->second;
      return NULL;
    }
    // this create a channel, and send back the pointer of it. add the new channel created in the map.
    // if the creation fail, return NULL.
    Channel* createChannel(const std::string& name)
    {
      t_channel ch;
      ch.s_name = name;
      ch.s_topic = "";
      ch.s_invOnl = false;
      ch.s_topicRest = false;
      ch.s_password = "";
      Channel* chan = new Channel(ch, 0);
      m_channels[name] = chan;
      return chan;
    }
};

#endif
