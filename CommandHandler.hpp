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
  
  void handleHelp(Client* client, const Command &cmd);
  void handlePass(Client* client, const Command &cmd);
public:
  CommandHandler();
  void dispatch(Client *client, const Command& cmd);
  ~CommandHandler(){}
};

#endif 
