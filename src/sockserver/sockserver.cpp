//
// Created by tobias on 29/09/16.
//

#include <string>
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include "rlutil/rlutil.h"
#include "sockserver/Server.h"
#include "sockophil/Validator.h"
#include "docopt.h"

#include "sockophil/Constants.h"

static const char USAGE[] =
    R"(sockserver.

    Usage:
      sockserver
      sockserver [-p <port> | --port=<port>] [-d <dir> | --dir=<dir>]
      sockserver (-h | --help)
      sockserver (--version)

    Options:
      -p <port>, --port=<port>  Port the server listens on [default: 1337].
      -d <dir>, --dir=<dir>     Relative path for Uploads [default: ./upload].
      -h, --help                Show this screen.
      --version                 Show version.
)";

int main(int argc, const char **argv) {

  int port = 0;
  std::string dir = "";
  std::string version = "sockserver 0.1";
  /* client pointer that is used later */
  std::shared_ptr<sockserver::Server> server;
  /* set the title of the terminal window */
  std::string username, password;
  std::cout << "Input Username: " << std::endl;
  std::cin >> username;
  std::cout << "Input password: " << std::endl;
  std::cin >> password;
  std::cout << server->LDAP_login(username, password) << std::endl;

  return 0;
}