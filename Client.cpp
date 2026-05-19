#include "Client.hpp"


Client::Client(int fd, std::string ip_address)
{
  this->m_fd = fd;
  this->m_ip_address = ip_address;
  std::cout << "Client is Set and Ready"<< std::endl;
}
int Client::GetFd() const 
{
  return m_fd;
}

t_ClientData& Client::getData()
{
  t_ClientData *cltdata;
  clt->
}
void Client::setData(t_ClientData& cltdata)
{
  m_data.m_nickname = cltdata.m_nickname;
  m_data.m_userName = cltdata.m_userName;
  m_data.m_realName = cltdata.m_realName;
  m_data.m_hostname = cltdata.m_hostname;
}
int Client::IsAuth()
{
  return cmd->m_Auth;
}

void Client::setAuth(bool auth)
{
  this->m_Auth = auth;
}
