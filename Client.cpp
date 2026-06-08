#include "Client.hpp"


Client::Client(int fd, std::string ip_address)
{
  this->m_fd = fd;
  this->m_ip_addr = ip_address;
  std::cout << "Client is Set and Ready"<< std::endl;
  std::cout << "nickname: " << m_data.m_nickname << std::endl;
}
int Client::getFd() const 
{
  return m_fd;
}

t_ClientData& Client::getData()
{
  return m_data;
}

void Client::setData(t_ClientData& cltdata)
{
  m_data.m_nickname = cltdata.m_nickname;
  m_data.m_username = cltdata.m_username;
  m_data.m_realname = cltdata.m_realname;
  m_data.m_hostname = cltdata.m_hostname;
}
int Client::IsAuth()
{
  return m_data.m_Auth;
}

void Client::setAuth(bool auth)
{
  this->m_data.m_Auth = auth;
}
