#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
struct command {
  std::string name;
  std::vector<std::string> params;
  bool hasTrailing;
  std::string trailing;
};


void parser(std::string input, command &cmd)
{
  size_t trailing = 0;
  if (!input.empty() && input[input.size() - 1] == '\n')
    input.erase(input.size() - 1);
  if (!input.empty() && input[input.size() - 1] == '\r')
    input.erase(input.size() - 1);
  size_t pos = input.find(' ');
  if (pos == std::string::npos)
    cmd.name = input;
  else {
    cmd.name = input.substr(0, pos);
    std::string rest = input.substr(pos + 1);
    trailing = rest.find(":");
    std::string param;
    if (trailing == std::string::npos)
    {
      std::stringstream ss(rest);
      while (ss >> param)
        cmd.params.push_back(param);
      cmd.hasTrailing = false;
    }
    else {
      std::string before = rest.substr(0, trailing);
      std::stringstream ss(before);
      while (ss >> param)
        cmd.params.push_back(param);
      cmd.trailing = rest.substr(trailing + 1);
      cmd.hasTrailing = true;
    }
  }
}

void printstruct(command& cmd)
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


int main(void)
{
  std::string input1 = "PASS secret123 param2 param3";
  std::string input2 = "PASS secrete123 para1 : this is message";
  command cmd;
  //parser(input1, cmd);
  parser(input2, cmd);
  printstruct(cmd);
  return 0;
}
