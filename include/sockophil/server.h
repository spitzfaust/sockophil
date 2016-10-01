//
// Created by Pommi on 9/30/16.
//

#ifndef SOCKOPHIL_SERVER_H
#define SOCKOPHIL_SERVER_H

#include <string>

class Server{
private:
  int port;
  std::string targetDir;
public:
  Server(int port, std::string targetDir);
  ~Server();


};

#endif //SOCKOPHIL_SERVER_H
