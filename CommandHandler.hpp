#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP 

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"
#include "tools.hpp"
// Forward declaration to avoid circular dependency
class Server;

class CommandHandler
{
private:
  typedef void (CommandHandler::*CmdFct)(Server*, Client*, const Command &);
  std::map<std::string, CmdFct> m_handle;
  // hna i addedd 3 member functions 
  // NICK , USER , QUIT , implementi men be3d
  void handleHelp(Server* server, Client* client, const Command &cmd);
  void handlePass(Server* server, Client* client, const Command &cmd);
  void handleNick(Server* server, Client* client, const Command &cmd);
  void handleUser(Server* server, Client* client, const Command &cmd);
  void handleQuit(Server* server, Client* client, const Command &cmd);
  void handleList(Server* server, Client* client, const Command &cmd);
  void handleJoin(Server* server, Client* client, const Command &cmd);
  
  void handlePrivmsg(Server* server, Client* client, const Command &cmd);
public:
  CommandHandler();
  void dispatch(Server* server, Client *client, const Command& cmd);
  void Checkregistration(Client* client);
  ~CommandHandler(){}
};

#endif 
