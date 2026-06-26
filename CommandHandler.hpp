#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP 

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"
#include "tools.hpp"

class CommandHandler
{
private:
  typedef void (CommandHandler::*CmdFct)(Client*, const Command &);
  std::map<std::string, CmdFct> m_handle;
  // hna i addedd 3 member functions 
  // NICK , USER , QUIT , implementi men be3d
  void handleHelp(Client* client, const Command &cmd);
  void handlePass(Client* client, const Command &cmd);
  void handleNick(Client* client, const Command &cmd);
  void handleUser(Client* client, const Command &cmd);
  void handleQuit(Client* client, const Command &cmd);
public:
  CommandHandler();
  void dispatch(Client *client, const Command& cmd);
  ~CommandHandler(){}
};

#endif 
