#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Client.hpp"

class Channel {
private:
    std::string m_name;
    std::string m_topic;
    std::map<int, Client*> m_members;   
    std::map<int, Client*> m_operators;  

    bool m_inviteOnly;       
    bool m_topicRestricted; 
    std::string m_password;  
    size_t m_userLimit;     

public:
    Channel(const std::string& name);
    ~Channel();

    
    void addClient(Client* client)
	{
		m_members[client->getFd()] = client;
	}
    void removeClient(Client* client)
	{

	}
    bool hasClient(Client* client) const;
    {
		
	}
    
    void addOperator(Client* client);
    void removeOperator(Client* client);
    bool isOperator(Client* client) const;

   
    void broadcast(const std::string& message, Client* sender);
    
    
};

#endif