#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <string>
#include <vector>
#include <map>


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

};

#endif // !i
