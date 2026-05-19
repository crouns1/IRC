#include "CommandHandler.hpp"



CommandHandler::CommandHandler()
{
  m_handler["PASS"] = &CommandHandler::handlePass;
}

void CommandHandler::dispatch(Client *client, const Command& cmd)
{
  std::map<std::string, CmdFct>::iterator it = m_handler.find(cmd.name);
  if (it != m_handler.end())
    (this->*(it->second))(client, cmd);
  else 
    client->sendMessage("421" + cmd.name + " :Unknown Command");
}

void CommandHandler::handlerPass(Clien *client, const Command& cmd)
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

  std::string pwd;
  if (cmd.has_trailing && !cmd.trailing.empty())
    pwd = cmd.trailing;
  else 
    pwd = cmd.params[0];
  
  if (true) // compare the password with the server one [TODO]
    client->setAuth(true);
  else {
    client->sendMessage("464 : Password incorrect");
  }
    
}


