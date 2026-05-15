#include "Client.hpp"



Client::Client(int fd, std::string ip_address) : m_fd(fd), m_ip_addr(ip_address)
{
  try {
    if (m_fd < 0)
      throw std::Error("fd is negative");
  }
  
}
