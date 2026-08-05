#include "Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int ac, char **av)
{
  if (ac != 3) 
  {
    std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
    return 1;
  }
  
  int port = std::atoi(av[1]);
  if (port <= 0 || port > 65535) 
  {
    std::cerr << "Invalid port: " << av[1] << std::endl;
    return 1;
  }
  std::string password = av[2];
  if (password.empty())
  {
    std::cerr << "Password cannot be empty" << std::endl;
    return 1;
  }
  
  Server server(port, password);
  server.run();
  
  return 0;
}