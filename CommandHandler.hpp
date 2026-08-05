#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP 

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"
#include "tools.hpp"

class Server;

class CommandHandler
{
private:
  typedef void (CommandHandler::*CmdFct)(Server*, Client*, const Command &);
  std::map<std::string, CmdFct> m_handle;
  
  void handleHelp(Server* server, Client* client, const Command &cmd);
  void handlePass(Server* server, Client* client, const Command &cmd);
  void handleNick(Server* server, Client* client, const Command &cmd);
  void handleUser(Server* server, Client* client, const Command &cmd);
  void handleQuit(Server* server, Client* client, const Command &cmd);
  void handleList(Server* server, Client* client, const Command &cmd);
  void handleJoin(Server* server, Client* client, const Command &cmd);
  void handlePart(Server* server, Client* client, const Command &cmd);
  void handleTopic(Server* server, Client* client, const Command &cmd);
  void handleKick(Server* server, Client* client, const Command &cmd);
  void handleInvite(Server* server, Client* client, const Command &cmd);
  void handleMode(Server* server, Client* client, const Command &cmd);
  void handlePing(Server* server, Client* client, const Command &cmd);
  void handleCap(Server* server, Client* client, const Command &cmd);
  
  void handlePrivmsg(Server* server, Client* client, const Command &cmd);
public:
  CommandHandler();
  void dispatch(Server* server, Client *client, const Command& cmd);
  void Checkregistration(Server* server, Client* client);
  ~CommandHandler(){}
};

#endif 
