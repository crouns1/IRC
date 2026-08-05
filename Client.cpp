#include "Client.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "tools.hpp"

Client::Client(int fd, std::string ip_address)
{
  this->m_fd = fd;
  this->m_ip_addr = ip_address;
  this->m_data.m_hostname = ip_address;
  this->m_disconnect = false;
  std::cout << "Client is Set and Ready"<< std::endl;
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
// had lpart rah kenti dayrha f lclass client 
// ms knti dayrha katsift msg l stdout bhal hka sendmsg { "sending msg to client << m_fd" << ekatbee3 lmsg}
// normallement khass lmsg itsift directly l client machui output
// messages are queued and flushed by the server (non-blocking writes)
int Client::sendMessage(const std::string& message) {
    m_write_buffer += message + "\r\n";
    std::cout << "Queueing to client " << m_fd << ": " << BLUE << message << std::endl;
    return 0;
}

// Add these to the bottom of Client.cpp

std::string Client::getReadBuffer() const {
    return m_read_buffer;
}

void Client::setReadBuffer(const std::string& buffer) {
    m_read_buffer = buffer;
}

std::string& Client::getWriteBuffer() {
    return m_write_buffer;
}

void Client::setWriteBuffer(const std::string& buffer) {
    m_write_buffer = buffer;
}

void Client::setDisconnect(bool disconnect) {
    m_disconnect = disconnect;
}

bool Client::shouldDisconnect() const {
    return m_disconnect;
}
