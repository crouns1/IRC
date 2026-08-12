#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/select.h>
#include "Client.hpp"
#include "Tools.hpp"
#include "CommandHandler.hpp"
#include "Channel.hpp"

class Server {
private:
    int m_port;
    std::string m_serverPassword;
    std::string m_serverName;
    int m_serverFd;
    std::map<std::string, Channel*> m_channels;

    std::map<int, Client*> m_clients;


    fd_set m_readFds;
    fd_set m_writeFds;
    int m_maxFd;

    CommandHandler m_cmdHandler;


    bool initSocket();

    void acceptNewClient();


    void handleClientData(int clientFd);


    void cleanupClient(int clientFd);

    bool flushClientWrites(int clientFd);


    void sendResponse(int clientFd, const std::string& response);

public:
    Server(int port, const std::string& password);
    ~Server();


    void removeChannel(const std::string& name);
    Channel* getChannel(const std::string& name);
    Channel* createChannel(const std::string& name);


    void run();

 
    bool validatePassword(const std::string& password);


    bool isNicknameInUse(const std::string& nickname);
    bool isNicknameInUse(const std::string& nickname, Client* exclude);


    Client* getClient(int fd);

    Client* getClientByNickname(const std::string& nickname);


    void removeClient(int fd);
    const std::string& getServerName() const;


    void removeClientFromChannels(Client* client, const std::string& reason);


    void broadcastToChannelsOf(Client* client, const std::string& message);
};

#endif
