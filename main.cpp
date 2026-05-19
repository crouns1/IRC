#include "CommandHandler.hpp"
#include "tools.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
  


void fillClient(Client *client) // this should be partially with the server. 
{
  t_ClientData cltdata;
  cltdata->m_nickname = "NickName";
  cltdata->m_userName = "UserName";
  cltdata->m_realName = "RealName";
  cltdata->m_hostname = "1.1.1.1.1";
  client->setData(cmd);
}

int main()
{
  std::string input = "PASS param1 param2 param3: this is a text";
  Client client;
  fillClient(&client);
  command cmd = parser(input);
  return 0;
}
