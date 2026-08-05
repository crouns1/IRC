#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#define RESET   "\033[0m"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BG_RED  "\033[41m"   
struct Command {
  std::string name;
  std::string prefix;
  std::vector<std::string> params;
  bool hasTrailing;
  std::string trailing;
};


Command parser(std::string input);
void printstruct(Command& cmd); 
#endif
