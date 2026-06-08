#include "CommandHandler.hpp"
#include "tools.hpp"

CommandHandler::CommandHandler()
{
  m_handle["HELP"] = &CommandHandler::handleHelp;
  m_handle["PASS"] = &CommandHandler::handlePass;
}
void CommandHandler::handleHelp(Client* client, const Command &cmd)
{
  if (cmd.params.empty())
  {
    
    client->sendMessage("Available commands: PASS, HELP");
    client->sendMessage("The command PASS is used to authenticate the client");
    client->sendMessage("The command HELP is used to get the list of available commands");
  }
  else if (cmd.params[0] == "PASS")
  {
    client->sendMessage("The command PASS is used to authenticate the client");
    client->sendMessage("Usage: PASS <password>");
  }
  else if (cmd.params[0] == "HELP")
    client->sendMessage("The command HELP is used to get the list of available commands");
  else 
    client->sendMessage("421 " + cmd.params[0] + " :Unknown command"); 
}
void CommandHandler::dispatch(Client *client, const Command& cmd)
{
  std::map<std::string, CmdFct>::iterator it = m_handle.find(cmd.name);
  if (it != m_handle.end())
    (this->*(it->second))(client, cmd);
  else 
    client->sendMessage("421" + cmd.name + " :Unknown Command");
}

void CommandHandler::handlePass(Client *client, const Command& cmd)
{
  if (client->IsAuth()) // The client is already in !
  {
    client->sendMessage("462 : No need to sign up Queen/king ");
    return ; 
  }
  if (cmd.params.empty())
  {
    client->sendMessage("461 PASS : password missing");
    return ;
  }
  else {
    client->sendMessage("OK PASS command received");
  }

  std::string pwd;
  if (cmd.hasTrailing && !cmd.trailing.empty())
    pwd = cmd.trailing;
  else 
    pwd = cmd.params[0];
  
  if (true) // compare the password with the server one [TODO]
    client->setAuth(true);
  else {
    client->sendMessage("464 : Password incorrect");
  }
}


