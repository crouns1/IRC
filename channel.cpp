#include "channel.hpp"

Channel::Channel(const t_channel s_ch, size_t userlimit)
{
  m_ch = s_ch;
  m_userLimit = userlimit;
  m_creationTime = std::time(NULL);
}
Channel::~Channel()
{}

void Channel::broadcast(const std::string& message, Client* sender)
{
  for (std::map<int, Client*>::iterator it = m_members.begin(); it != m_members.end(); it++)
  {
    if (sender == NULL || it->second != sender)
      it->second->sendMessage(message);
  }
}

void Channel::addOperator(Client* client)
{
  m_operators[client->getFd()] = client;
}

void Channel::removeOperator(Client* client)
{
  m_operators.erase(client->getFd());
}

bool Channel::isOperator(Client* client) const
{
  return m_operators.find(client->getFd()) != m_operators.end();
}

bool Channel::isEmpty() const
{
  return m_members.empty();
}

int Channel::getMemberCount() const
{
  return static_cast<int>(m_members.size());
}

const std::string& Channel::getName() const
{
  return m_ch.s_name;
}

std::vector<std::string> Channel::getMemberList() const
{
  std::vector<std::string> list;
  for (std::map<int, Client*>::const_iterator it = m_members.begin(); it != m_members.end(); ++it)
  {
    if (isOperator(it->second))
      list.push_back("@" + it->second->getData().m_nickname);
    else
      list.push_back(it->second->getData().m_nickname);
  }
  return list;
}

bool Channel::isInviteOnly() const
{
  return m_ch.s_invOnl;
}

void Channel::setInviteOnly(bool value)
{
  m_ch.s_invOnl = value;
}

bool Channel::isTopicRestricted() const
{
  return m_ch.s_topicRest;
}

void Channel::setTopicRestricted(bool value)
{
  m_ch.s_topicRest = value;
}

const std::string& Channel::getPassword() const
{
  return m_ch.s_password;
}

void Channel::setPassword(const std::string& pass)
{
  m_ch.s_password = pass;
}

size_t Channel::getUserLimit() const
{
  return m_userLimit;
}

void Channel::setUserLimit(size_t limit)
{
  m_userLimit = limit;
}

const std::string& Channel::getTopic() const
{
  return m_ch.s_topic;
}

void Channel::setTopic(const std::string& topic)
{
  m_ch.s_topic = topic;
}

time_t Channel::getCreationTime() const
{
  return m_creationTime;
}

void Channel::addInvite(Client* client)
{
  m_invited[client->getFd()] = client;
}

void Channel::removeInvite(Client* client)
{
  m_invited.erase(client->getFd());
}

bool Channel::isInvited(Client* client) const
{
  return m_invited.find(client->getFd()) != m_invited.end();
}
