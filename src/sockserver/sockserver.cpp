//
// Created by tobias on 29/09/16.
//

#include <string>
#include <iostream>
#include <stdlib.h>
#include "rlutil/rlutil.h"
#include "docopt.h"

<<<<<<< HEAD
int main() {
  rlutil::setBackgroundColor(rlutil::MAGENTA);
  rlutil::setColor(rlutil::BLUE);
  std::cout << "Hello World";
=======
#include "sockophil/constants.h"

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

int main(int argc, const char** argv)
{
    std::map<std::string, docopt::value> args
            = docopt::docopt(USAGE,
                             { argv + 1, argv + argc },
                             true,               // show help if requested
                             "socketserver 0.1");  // version string

    for(auto const& arg : args) {
        std::cout << arg.first << "  " <<  arg.second << std::endl;
    }
    try {
        auto port = std::stoi(args["--port"].asString());
        if (port < sockophil::MIN_PORT) {
            std::cerr << "Given port: " << args["--port"].asString() << std::endl;
            std::cerr << "The port has to be bigger than " << sockophil::MIN_PORT << "." << std::endl;
            return EXIT_FAILURE;
        } else if (port > sockophil::MAX_PORT) {
            std::cerr << "Given port: " << args["--port"].asString() << std::endl;
            std::cerr << "The port has to be smaller than " << sockophil::MAX_PORT << "." << std::endl;
            return EXIT_FAILURE;
        }
    } catch (std::invalid_argument e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is not a valid number and can not be a port." << std::endl;
        return EXIT_FAILURE;
    } catch (std::out_of_range e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is too big to be a valid port." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
>>>>>>> 1e11d3ebf8c091d76b65764a30871b4174b292d1
}