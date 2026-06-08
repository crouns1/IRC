#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

struct Command {
  std::string name;
  std::vector<std::string> params;
  bool hasTrailing;
  std::string trailing;
};


Command parser(std::string input);
void printstruct(Command& cmd); 
#endif
