#include "CommandHandler.hpp"
#include "tools.hpp"
#include "Server.hpp"

CommandHandler::CommandHandler()
{
  // same as usall
  // same as u , i mapped command names to funcs that handle those commands
   
  m_handle["HELP"] = &CommandHandler::handleHelp;
  m_handle["PASS"] = &CommandHandler::handlePass;
  m_handle["NICK"] = &CommandHandler::handleNick;
  m_handle["USER"] = &CommandHandler::handleUser;
  m_handle["QUIT"] = &CommandHandler::handleQuit;
}
// here i implemented help that list all the commands that is availlable
// HELP / PASS 

void CommandHandler::handleHelp(Server* server, Client* client, const Command &cmd)
{
  (void)server; // Not used in help command
  
  if (cmd.params.empty())
  {
    
    client->sendMessage("only HELP and PASS is availlable");
    client->sendMessage("PASS is used to authenticate the client");
    client->sendMessage("HELP is used to get the list of available commands");
  }
  else if (cmd.params[0] == "PASS")
  {
    client->sendMessage("PASS is used to authenticate the client");
    client->sendMessage("Usage: PASS <password>");
  }
  else if (cmd.params[0] == "HELP")
    client->sendMessage("HELP is used to get the list of available commands");
  else 
    client->sendMessage("421 " + cmd.params[0] + " :Unknown command"); 
}
void CommandHandler::dispatch(Server* server, Client *client, const Command& cmd)
{
  std::map<std::string, CmdFct>::iterator it = m_handle.find(cmd.name);
  if (it != m_handle.end())
    (this->*(it->second))(server, client, cmd);
  else 
    client->sendMessage("421" + cmd.name + " :Unknown Command");
}
// for the PASS cmd
// it compare betwene the stored password which we give it as arg 3 --> ./irc "port" "pass" 
// with the client password 
// if password correct --> password accepted 
// but i didnt check the arg after PASS pasworrd "?"
// if exist "he should not exist at all"
void CommandHandler::handlePass(Server* server, Client *client, const Command& cmd)
{
  if (client->IsAuth()) // The client is already authenticated
  {
    client->sendMessage("462 :You may not reregister");
    return ; 
  }
  
  // Strict validation: check if password parameter is provided
  if (cmd.params.empty() && (cmd.trailing.empty() || !cmd.hasTrailing))
  {
    client->sendMessage("461 PASS :Not enough parameters");
    return ;
  }

  std::string pwd;
  if (cmd.hasTrailing && !cmd.trailing.empty())
    pwd = cmd.trailing;
  else 
    pwd = cmd.params[0];
  
  // Strict password validation against server password
  if (!server->validatePassword(pwd)) {
    client->sendMessage("464 :Password incorrect");
    return;
  }
  
  // Mark password as validated
  t_ClientData& data = client->getData();
  data.m_has_pwd = true;
  
  client->sendMessage("NOTICE * :Password accepted");
}


// db khassek timplementi had nick ...



void CommandHandler::handleNick(Server* server, Client* client, const Command& cmd)
{
  (void)server;
  (void)cmd; 
  client->sendMessage("NICK :Not implemented yet");
}

void CommandHandler::handleUser(Server* server, Client* client, const Command& cmd)
{
  (void)server;
  (void)cmd;
  client->sendMessage("USER :Not implemented yet");
}

void CommandHandler::handleQuit(Server* server, Client* client, const Command& cmd)
{
  (void)server;
  (void)cmd; 
  client->sendMessage("QUIT: DOSENT IMPLEMENTED YET");
}