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
  std::string version = "sockserver 2.0";
  /* client pointer that is used later */
  std::shared_ptr<sockserver::Server> server;
  /* set the title of the terminal window */
  rlutil::setConsoleTitle(version);
  std::map<std::string, docopt::value> args
      = docopt::docopt(USAGE,
                       {argv + 1, argv + argc},
                       true,               // show help if requested
                       version);  // version string

  try {
    /* convert string to integer */
    port = std::stoi(args["--port"].asString());
    /* check if valid port was entered */
    if (!sockophil::Validator::is_valid_port(port)) {
      std::cerr << "Given port: " << args["--port"].asString() << std::endl;
      std::cerr << "The port has to be bigger than " << sockophil::MIN_PORT << "." << std::endl;
      std::cerr << "The port has to be smaller than " << sockophil::MAX_PORT << "." << std::endl;
      return -1;
    }
  } catch (const std::invalid_argument &e) {
    std::cerr << "Given port: " << args["--port"].asString() << std::endl;
    std::cerr << "The argument is not a valid number and can not be a port." << std::endl;
    return -1;
  } catch (const std::out_of_range &e) {
    std::cerr << "Given port: " << args["--port"].asString() << std::endl;
    std::cerr << "The argument is too big to be a valid port." << std::endl;
    return -1;
  }
  dir = args["--dir"].asString();

  /* create a new Server object */
  server = std::make_shared<sockserver::Server>(port, dir);
  server->run();

  return 0;

}