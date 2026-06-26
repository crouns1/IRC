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
  std::string password = av[2];
  
  Server server(port, password);
  server.run();
  
  return 0;
}