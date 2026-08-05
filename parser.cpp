#include "tools.hpp"
#include <cctype>

static void uppercase(std::string& str)
{
  for (size_t i = 0; i < str.size(); ++i)
    str[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));
}

Command parser(std::string input)
{
  Command cmd;
  cmd.hasTrailing = false;


  if (!input.empty() && input[input.size() - 1] == '\n')
    input.erase(input.size() - 1);
  if (!input.empty() && input[input.size() - 1] == '\r')
    input.erase(input.size() - 1);

  if (!input.empty() && input[0] == ':')
  {
    size_t space = input.find(' ');
    if (space == std::string::npos)
    {
      cmd.prefix = input.substr(1);
      input.clear();
    }
    else
    {
      cmd.prefix = input.substr(1, space - 1);
      input.erase(0, space + 1);
    }
  }

  size_t colon = input.find(" :");
  if (colon != std::string::npos)
  {
    cmd.trailing = input.substr(colon + 2);
    cmd.hasTrailing = true;
    input.erase(colon);
  }

  std::stringstream ss(input);
  std::string token;
  if (ss >> token)
    cmd.name = token;
  while (ss >> token)
    cmd.params.push_back(token);

  uppercase(cmd.name);
  return cmd;
}

void printstruct(Command& cmd)
{
  std::cout << "Name: " << cmd.name << std::endl;
  std::cout << "Prams: ";
  for (std::vector<std::string>::iterator it = cmd.params.begin(); it != cmd.params.end(); it++)
  {
    std::cout << *it << " ";
  }
  std::cout<< std::endl;
  std::cout << "traling: " << cmd.hasTrailing << std::endl;
  if (cmd.hasTrailing)
  {
    std::cout << "trailing: " << cmd.trailing << std::endl;
  }
}


