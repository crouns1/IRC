#ifndef CLIENT_HPP
#define CLIENT_HPP 
#include <iostream>
#include <string> 
#include <vector>
#include <map>




typedef struct s_ClientData {
  bool m_has_pwd = false;
  bool m_has_nick = false;
  bool m_has_user = false;
  bool m_is_registred = false;

  std::string m_nickname;
  std::string m_username;
  std::string m_realname;
  std::string m_hostname; // just the ip 
} t_ClientData;

class Client {
private: 
  int m_fd;
  std::string m_ip_address;
  std::string m_read_buffer;
  std::string m_write_buffer;

  t_ClientData m_data;

  std::vector<Channel*> m_joined_channels;
public:
  Client(int fd, std::string ip_address);
  ~Client();

  int getFd() const;
  t_ClientData getData() const;
  void SetData(t_ClientData &data) const;
  std::string extractCommad(); 
  void addChannel(Channel *ch);
  void removeChannel(Channel *ch);
};

#endif
