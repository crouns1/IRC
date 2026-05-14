#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

typedef struct s_ChData {
  std::string m_name;
  std::string m_topic;
  std::string m_password;
  size_t m_userLimit;

  bool m_isInviteOnly;
  bool m_isRestricted;
  bool m_hasUserLimit;
  bool m_hasPass;
  s_ChData(): m_isInviteOnly(false), m_isRestricted(false),
              m_hasUserLimit(false), m_hasPass(false) {}
} t_ChData;

class Channel {
private:
  std::string m_name;
  t_ChData m_Chdata;
  std::map<int, Client*> m_members;
  std::vector<std::string> m_invitedUsers;
public:
  Channel(const std::string &name);
  ~Channel();
  t_ChData& GetChData();
  void addClient(Client *client);
  void removeClient(Client *client);
  bool isMember(CLient* client) const;

  void inviteUser(const std::string& nickname);
  bool isInvited(const std::string& nickname) const;
  void removeInvite(const std::string& nickname);

  void broadcast(const std::string& message, Client* sender);

};

#endif // !i
