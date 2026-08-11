#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "Client.hpp"

struct t_channel {
  std::string s_name;
  std::string s_topic;
  bool s_invOnl;
  bool s_topicRest;
  std::string s_password;
};

class Channel {
private:
    t_channel m_ch;
    std::map<int, Client*> m_members;
    std::map<int, Client*> m_operators;
    std::map<int, Client*> m_invited;

    size_t m_userLimit;
    time_t m_creationTime;

public:
    Channel(const t_channel s_ch, size_t userlimit);
    ~Channel();

    void addClient(Client* client);
    void removeClient(Client* client);
    bool hasClient(Client* client) const;

    void addOperator(Client* client);
    void removeOperator(Client* client);
    bool isOperator(Client* client) const;

    bool isEmpty() const;
    int getMemberCount() const;
    const std::string& getName() const;
    std::vector<std::string> getMemberList() const;

    bool isInviteOnly() const;
    void setInviteOnly(bool value);
    bool isTopicRestricted() const;
    void setTopicRestricted(bool value);
    const std::string& getPassword() const;
    void setPassword(const std::string& pass);
    size_t getUserLimit() const;
    void setUserLimit(size_t limit);
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);
    time_t getCreationTime() const;

    void addInvite(Client* client);
    void removeInvite(Client* client);
    bool isInvited(Client* client) const;

    void broadcast(const std::string& message, Client* sender);
};

#endif
