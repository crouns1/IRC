#ifndef TOOLS_HPP
#define TOOLS_HPP

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


command parser(std::string input);
void printstruct(command& cmd); 
#endif
