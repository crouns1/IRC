#ifndef CLIENT_HPP
#define CLIENT_HPP 
#include <iostream>
#include <string> 
#include <vector>
#include <map>

typedef struct s_ClientData {
  bool m_has_pwd;
  bool m_has_nick;
  bool m_has_user;
  bool m_Auth;

  std::string m_nickname;
  std::string m_username;
  std::string m_realname;
  std::string m_hostname; // just the ip 
  s_ClientData() : m_has_pwd(false), m_has_nick(false), m_has_user(false), m_Auth(false),
                   m_nickname(""), m_username(""), m_realname(""), m_hostname("") {}
} t_ClientData;

class Client {
private: 
  int m_fd;
  std::string m_ip_addr;
  t_ClientData m_data;
  std::string m_read_buffer;
  std::string m_write_buffer;
  bool m_disconnect;


  //std::vector<Channel*> m_joined_channels;
public:
  Client(int fd, std::string ip_address);
  ~Client(){};

  int getFd() const; // DONE
  int IsAuth(); // DONE 
  t_ClientData& getData(); // DONE
  void setAuth(bool auth); // DONE
  void  setData(t_ClientData& cltdata);
  std::string getReadBuffer() const;
  void setReadBuffer(const std::string& buffer);
  std::string& getWriteBuffer();
  void setWriteBuffer(const std::string& buffer);
  void setDisconnect(bool disconnect);
  bool shouldDisconnect() const;
  // ALERT ""change""
  int sendMessage(const std::string& message);
  //std::string extractCommad();
  //void addChannel(Channel *ch);
  //void removeChannel(Channel *ch);
};

#endif
