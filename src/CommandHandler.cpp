#include "CommandHandler.hpp"
#include "Tools.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <cctype>
#include <cstring>

static bool isValidNickname(const std::string& nick)
{
  if (nick.empty() || nick.size() > 9)
    return false;
  for (size_t i = 0; i < nick.size(); ++i)
  {
    char c = nick[i];
    if (!(std::isalnum(static_cast<unsigned char>(c)) || std::strchr("[]\\`_^{|}-", c)))
      return false;
  }
  return true;
}

std::string CommandHandler::getPrefix(Client* client)
{
  t_ClientData& d = client->getData();
  std::string nick;
  std::string host;

  if (d.m_nickname.empty())
    nick  = "*";
  else
    nick = d.m_nickname;

  if (d.m_hostname.empty())
    host = "127.0.0.1";
  else
    host = d.m_hostname;
  return ":" + nick + "!" + d.m_username + "@" + host;
}

CommandHandler::CommandHandler()
{
  m_handle["HELP"] = &CommandHandler::handleHelp;
  m_handle["PASS"] = &CommandHandler::handlePass;
  m_handle["NICK"] = &CommandHandler::handleNick;
  m_handle["USER"] = &CommandHandler::handleUser;
  m_handle["QUIT"] = &CommandHandler::handleQuit;
  m_handle["PRIVMSG"] = &CommandHandler::handlePrivmsg;
  m_handle["JOIN"] = &CommandHandler::handleJoin;
  m_handle["TOPIC"] = &CommandHandler::handleTopic;
  m_handle["KICK"] = &CommandHandler::handleKick;
  m_handle["INVITE"] = &CommandHandler::handleInvite;
  m_handle["MODE"] = &CommandHandler::handleMode;
}

void CommandHandler::dispatch(Server* server, Client *client, const Command& cmd)
{
  if (cmd.name.empty())
    return;
  std::map<std::string, CmdFct>::iterator it = m_handle.find(cmd.name);
  if (it != m_handle.end())
    (this->*(it->second))(server, client, cmd);
  else
    client->sendMessage("421 " + cmd.name + " :Unknown Command");
}

void CommandHandler::handlePass(Server* server, Client *client, const Command& cmd)
{
  if (client->IsAuth())
  {
    client->sendMessage("462 :You may not reregister");
    return ;
  }

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

  if (!server->validatePassword(pwd)) {
    client->sendMessage("464 :Password incorrect");
    return;
  }

  t_ClientData& data = client->getData();
  data.m_has_pwd = true;

  client->sendMessage("NOTICE * :Password accepted");
  Checkregistration(server, client);
}

void CommandHandler::Checkregistration(Server* server, Client* client)
{
  t_ClientData& data = client->getData();
  if(client->IsAuth())
    return;
  if(data.m_has_nick && data.m_has_user && data.m_has_pwd)
  {
    data.m_Auth = true;
    std::string nick = data.m_nickname;
    std::string host;
    if(data.m_hostname.empty())
      host = "127.0.0.1";
    else
      host = data.m_hostname;
    std::string srv = server->getServerName();

    client->sendMessage(":" + srv + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + data.m_username + "@" + host);
    client->sendMessage(":" + srv + " 002 " + nick + " :Your host is " + srv);
  }
}

void CommandHandler::handleNick(Server* server, Client* client, const Command& cmd)
{
  t_ClientData& data = client->getData();

  std::string nickname;
  if (!cmd.params.empty())
    nickname = cmd.params[0];
  else if (cmd.hasTrailing && !cmd.trailing.empty())
    nickname = cmd.trailing;
  if (nickname.empty())
  {
    client->sendMessage("431 :No nickname given");
    return;
  }
  if (!isValidNickname(nickname)) {
    client->sendMessage("432 * " + nickname + " :Erroneous nickname");
    return;
  }
  if (server->isNicknameInUse(nickname, client)) {
    client->sendMessage("433 * " + nickname + " :Nickname is already in use");
    return;
  }

  bool changing = data.m_has_nick;
  std::string oldNick = data.m_nickname;
  data.m_nickname = nickname;
  data.m_has_nick = true;

  if (changing)
  {
    std::string host;
    if (data.m_hostname.empty())
      host =  "127.0.0.1";
    else
      host = data.m_hostname;
    server->broadcastToChannelsOf(client, ":" + oldNick + "!" + data.m_username + "@" + host + " NICK :" + nickname);
  }
  Checkregistration(server, client);
}

void CommandHandler::handleUser(Server* server, Client* client, const Command& cmd)
{
  t_ClientData& data = client->getData();

  if (client->IsAuth()) {
    client->sendMessage("462 ::Unauthorized command (already registered");
    return;
  }
  if (cmd.params.size() < 3 || (!cmd.hasTrailing && cmd.params.size() < 4)) {
    client->sendMessage("461 USER :Not enough parameters");
    return;
  }
  data.m_username = cmd.params[0];

  if (cmd.hasTrailing)
    data.m_realname = cmd.trailing;
  else
    data.m_realname = cmd.params[3];

  data.m_has_user = true;

  Checkregistration(server, client);
}


void CommandHandler::handlePrivmsg(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }

  if (cmd.params.empty()) {
    client->sendMessage("411 :No recipient given (PRIVMSG)");
    return;
  }
  if (!cmd.hasTrailing || cmd.trailing.empty()) {
    client->sendMessage("412 :No text to send");
    return;
  }

  std::string target = cmd.params[0];
  std::string fullMessage = getPrefix(client) + " PRIVMSG " + target + " :" + cmd.trailing;

  if (!target.empty() && target[0] == '#') {
    Channel* channel = server->getChannel(target);
    if (channel == NULL) {
      client->sendMessage("401 " + target + " :No such nick/channel");
      return;
    }
    if (!channel->hasClient(client)) {
      client->sendMessage("404 " + target + " :Cannot send to channel");
      return;
    }
    channel->broadcast(fullMessage, client);
    return;
  }

  Client* targetClient = server->getClientByNickname(target);
  if (!targetClient) {
    client->sendMessage("401 " + target + " :No such nick/channel");
    return;
  }
  targetClient->sendMessage(fullMessage);
}

void CommandHandler::handleJoin(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }
  if (cmd.params.empty()) {
    client->sendMessage("461 JOIN :Not enough parameters");
    return;
  }

  std::string ch_name = cmd.params[0];
  std::string key;
  if (cmd.params.size() > 1)
    key = cmd.params[1];

  if (ch_name.empty() || ch_name[0] != '#' || ch_name.size() > 50) {
    client->sendMessage("403 " + ch_name + " :No such channel");
    return;
  }

  Channel* channel = server->getChannel(ch_name);

  if (channel != NULL)
  {
    if (channel->hasClient(client))
      return;
    if (channel->isInviteOnly() && !channel->isInvited(client)) {
      client->sendMessage("473 " + ch_name + " :Cannot join channel (+i)");
      return;
    }
    if (!channel->getPassword().empty() && channel->getPassword() != key) {
      client->sendMessage("475 " + ch_name + " :Cannot join channel (+k)");
      return;
    }
    if (channel->getUserLimit() > 0 && channel->getMemberCount() >= static_cast<int>(channel->getUserLimit())) {
      client->sendMessage("471 " + ch_name + " :Cannot join channel (+l)");
      return;
    }
    channel->removeInvite(client);
  }
  else
  {
    channel = server->createChannel(ch_name);
  }

  bool first = channel->isEmpty();
  channel->addClient(client);
  if (first)
    channel->addOperator(client);

  channel->broadcast(getPrefix(client) + " JOIN :" + ch_name, NULL);

  std::string names;
  std::vector<std::string> members = channel->getMemberList();
  for (size_t i = 0; i < members.size(); ++i) {
    if (i > 0)
      names += " ";
    names += members[i];
  }

  std::string serverName = server->getServerName();
  std::string nick = client->getData().m_nickname;
  client->sendMessage(":" + serverName + " 353 " + nick + " = " + ch_name + " :" + names);
  client->sendMessage(":" + serverName + " 366 " + nick + " " + ch_name + " :End of /NAMES list.");
}

void CommandHandler::handleTopic(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }
  if (cmd.params.empty()) {
    client->sendMessage("461 TOPIC :Not enough parameters");
    return;
  }

  std::string ch_name = cmd.params[0];
  std::string nick = client->getData().m_nickname;
  Channel* channel = server->getChannel(ch_name);
  if (channel == NULL) {
    client->sendMessage("403 " + ch_name + " :No such channel");
    return;
  }
  if (!channel->hasClient(client)) {
    client->sendMessage("442 " + ch_name + " :You're not on that channel");
    return;
  }

  std::string srv = server->getServerName();

  if (cmd.params.size() == 1 && !cmd.hasTrailing) {
    if (channel->getTopic().empty())
      client->sendMessage(":" + srv + " 331 " + nick + " " + ch_name + " :No topic is set");
    else
      client->sendMessage(":" + srv + " 332 " + nick + " " + ch_name + " :" + channel->getTopic());
    return;
  }

  std::string newTopic;
  if (cmd.hasTrailing)
    newTopic = cmd.trailing;
  else if (cmd.params.size() > 1)
    newTopic = cmd.params[1];

  if (channel->isTopicRestricted() && !channel->isOperator(client)) {
    client->sendMessage("482 " + ch_name + " :You're not channel operator");
    return;
  }

  channel->setTopic(newTopic);
  channel->broadcast(getPrefix(client) + " TOPIC " + ch_name + " :" + newTopic, NULL);
}

void CommandHandler::handleKick(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }
  if (cmd.params.size() < 2) {
    client->sendMessage("461 KICK :Not enough parameters");
    return;
  }

  std::string ch_name = cmd.params[0];
  std::string targetNick = cmd.params[1];
  std::string reason = client->getData().m_nickname;
  if (cmd.hasTrailing && !cmd.trailing.empty())
    reason = cmd.trailing;

  Channel* channel = server->getChannel(ch_name);
  if (channel == NULL) {
    client->sendMessage("403 " + ch_name + " :No such channel");
    return;
  }
  if (!channel->hasClient(client)) {
    client->sendMessage("442 " + ch_name + " :You're not on that channel");
    return;
  }
  if (!channel->isOperator(client)) {
    client->sendMessage("482 " + ch_name + " :You're not channel operator");
    return;
  }

  Client* target = server->getClientByNickname(targetNick);
  if (target == NULL) {
    client->sendMessage("401 " + targetNick + " :No such nick/channel");
    return;
  }
  if (!channel->hasClient(target)) {
    client->sendMessage("441 " + targetNick + " " + ch_name + " :They aren't on that channel");
    return;
  }

  channel->broadcast(getPrefix(client) + " KICK " + ch_name + " " + targetNick + " :" + reason, NULL);
  channel->removeClient(target);
  channel->removeOperator(target);
  if (channel->isEmpty())
    server->removeChannel(ch_name);
}

void CommandHandler::handleInvite(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }
  if (cmd.params.size() < 2) {
    client->sendMessage("461 INVITE :Not enough parameters");
    return;
  }

  std::string targetNick = cmd.params[0];
  std::string ch_name = cmd.params[1];
  std::string nick = client->getData().m_nickname;
  std::string srv = server->getServerName();
  Channel* channel = server->getChannel(ch_name);
  if (channel == NULL) {
    client->sendMessage("403 " + ch_name + " :No such channel");
    return;
  }
  if (!channel->hasClient(client)) {
    client->sendMessage("442 " + ch_name + " :You're not on that channel");
    return;
  }

  Client* target = server->getClientByNickname(targetNick);
  if (target == NULL) {
    client->sendMessage("401 " + targetNick + " :No such nick/channel");
    return;
  }
  if (channel->hasClient(target)) {
    client->sendMessage("443 " + targetNick + " " + ch_name + " :is already on channel");
    return;
  }
  if (channel->isInviteOnly() && !channel->isOperator(client)) {
    client->sendMessage("482 " + ch_name + " :You're not channel operator");
    return;
  }
  channel->addInvite(target);
  target->sendMessage(getPrefix(client) + " INVITE " + targetNick + " :" + ch_name);
  client->sendMessage(":" + srv + " 341 " + nick + " " + targetNick + " " + ch_name);
}

void CommandHandler::handleMode(Server* server, Client* client, const Command& cmd)
{
  if (!client->IsAuth()) {
    client->sendMessage("451 :You have not registered");
    return;
  }
  if (cmd.params.empty()) {
    client->sendMessage("461 MODE :Not enough parameters");
    return;
  }

  std::string target = cmd.params[0];
  std::string nick = client->getData().m_nickname;
  std::string srv = server->getServerName();

  if (target.empty() || target[0] != '#') {
    client->sendMessage("221 " + nick + " +");
    return;
  }

  Channel* channel = server->getChannel(target);
  if (channel == NULL) {
    client->sendMessage("403 " + target + " :No such channel");
    return;
  }
  if (!channel->hasClient(client)) {
    client->sendMessage("442 " + target + " :You're not on that channel");
    return;
  }

  if (cmd.params.size() == 1) {
    std::string modes = "+";
    std::string args;
    if (channel->isInviteOnly()) modes += "i";
    if (channel->isTopicRestricted()) modes += "t";
    if (!channel->getPassword().empty()) { modes += "k"; args += " " + channel->getPassword(); }
    if (channel->getUserLimit() > 0) {
      modes += "l";
      std::stringstream ss;
      ss << channel->getUserLimit();
      args += " " + ss.str();
    }
    client->sendMessage(":" + srv + " 324 " + nick + " " + target + " " + modes + args);
    std::stringstream ct;
    ct << channel->getCreationTime();
    client->sendMessage(":" + srv + " 329 " + nick + " " + target + " " + ct.str());
    return;
  }
  if (!channel->isOperator(client)) {
    client->sendMessage("482 " + target + " :You're not channel operator");
    return;
  }

  std::string modeStr = cmd.params[1];
  size_t paramIdx = 2;
  char sign = '+';
  std::string appliedModes;
  std::string appliedArgs;
  bool hasChange = false;
  char lastEmittedSign = 0;

  for (size_t i = 0; i < modeStr.size(); ++i) {
    char c = modeStr[i];
    if (c == '+' || c == '-') {
      sign = c;
      continue;
    }

    bool adding = (sign == '+');

    if (c == 'o') {
      if (paramIdx >= cmd.params.size()) continue;
      std::string targetNick = cmd.params[paramIdx++];
      Client* tgt = server->getClientByNickname(targetNick);
      if (tgt == NULL || !channel->hasClient(tgt)) {
        client->sendMessage("401 " + targetNick + " :No such nick/channel");
        continue;
      }
      if (adding)
        channel->addOperator(tgt);
      else
        channel->removeOperator(tgt);
      if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
      appliedModes += c;
      appliedArgs += " " + targetNick;
      hasChange = true;
    }
    else if (c == 'k') {
      if (adding) {
        if (paramIdx >= cmd.params.size()) continue;
        std::string pass = cmd.params[paramIdx++];
        channel->setPassword(pass);
        if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
        appliedModes += 'k';
        appliedArgs += " " + pass;
      }
      else {
        channel->setPassword("");
        if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
        appliedModes += 'k';
      }
      hasChange = true;
    }
    else if (c == 'l') {
      if (adding) {
        if (paramIdx >= cmd.params.size()) continue;
        std::stringstream ss(cmd.params[paramIdx++]);
        size_t limit;
        ss >> limit;
        channel->setUserLimit(limit);
        if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
        appliedModes += 'l';
        appliedArgs += " " + cmd.params[paramIdx - 1];
      }
      else {
        channel->setUserLimit(0);
        if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
        appliedModes += 'l';
      }
      hasChange = true;
    }
    else if (c == 'i') {
      channel->setInviteOnly(adding);
      if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
      appliedModes += c;
      hasChange = true;
    }
    else if (c == 't') {
      channel->setTopicRestricted(adding);
      if (sign != lastEmittedSign) { appliedModes += sign; lastEmittedSign = sign; }
      appliedModes += c;
      hasChange = true;
    }
    else {
      std::string unknown;
      unknown += sign;
      unknown += c;
      client->sendMessage("472 " + unknown + " :is unknown mode char to me");
    }
  }

  if (hasChange)
    channel->broadcast(getPrefix(client) + " MODE " + target + " " + appliedModes + appliedArgs, NULL);
}

void CommandHandler::handleQuit(Server* server, Client* client, const Command& cmd)
{
  std::string reason = "Client Quit";
  if (cmd.hasTrailing && !cmd.trailing.empty())
    reason = cmd.trailing;

  t_ClientData& data = client->getData();
  server->removeClientFromChannels(client, reason);
  client->sendMessage("ERROR :Closing Link: " + (data.m_nickname.empty() ? "*" : data.m_nickname) + " (" + reason + ")");
  client->setDisconnect(true);
}

void CommandHandler::handleHelp(Server* server, Client* client, const Command &cmd)
{
  (void)server;

  if (cmd.params.empty())
  {
    client->sendMessage("PASS <password>    - authenticate with the server password");
    client->sendMessage("NICK <nickname>    - set or change your nickname");
    client->sendMessage("USER <user> 0 * <realname> - set your username");
    client->sendMessage("JOIN <#channel>    - join or create a channel");
    client->sendMessage("PRIVMSG <target> :<text> - send a message to a user or channel");
    client->sendMessage("TOPIC <#channel> [topic] - view or set a channel topic");
    client->sendMessage("KICK <#channel> <nick> - kick a user from a channel");
    client->sendMessage("INVITE <nick> <#channel> - invite a user to a channel");
    client->sendMessage("MODE <#channel> [+|-][io k l t] - change channel modes");
    client->sendMessage("QUIT [message]     - disconnect from the server");
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


