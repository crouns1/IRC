#include "CommandHandler.hpp"
#include "tools.hpp"

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
  if (client->IsAuth()) // The client is already in which is authenticated
  {
    client->sendMessage("462 :No need to sign up Queen/king ");
    return ; 
  }
  // hna dret wht 3 checks in same statement  
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
  
  // hna password validation should be done at server level
  // The CommandHandler mat9derch t3ref server password
  // For now, we will  assume validation happens elsewhere
  // ghandiro mark l password li already provided
  t_ClientData& data = client->getData();
  data.m_has_pwd = true;
  
  client->sendMessage("BINGO mr ahmed :Password accepted");
}


// db khassek timplementi had nick ...



void CommandHandler::handleNick(Client* client, const Command& cmd)
{
  (void)cmd; 
  client->sendMessage("NICK :Not implemented yet");
}

void CommandHandler::handleUser(Client* client, const Command& cmd)
{
  (void)cmd;
  client->sendMessage("USER :Not implemented yet");
}

void CommandHandler::handleQuit(Client* client, const Command& cmd)
{
  (void)cmd; 
  client->sendMessage("QUIT: DOSENT IMPLEMENTED YET");
}