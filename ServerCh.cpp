#include "Server.hpp"
#include <vector>

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
