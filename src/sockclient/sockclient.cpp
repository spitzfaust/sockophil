//
// Created by tobias on 29/09/16.
//

#include <iostream>

#include <memory>
#include <sockclient/SocketCreationException.h>
#include <sockclient/SocketConnectionException.h>
#include "sockclient/Client.h"
#include "sockclient/CurrentDirectoryException.h"
#include "sockophil/Validator.h"
#include "rlutil/rlutil.h"
#include "docopt.h"

#include "sockophil/constants.h"


/**
 * @var USAGE holds information of the accepted arguments. Used to parse arguments by docopt
 */
static const char USAGE[] =
        R"(sockclient.

    Usage:
      sockclient
      sockclient [-p <port> | --port <port>] [-i <ip> | --ip <ip>]
      sockclient (-h | --help)
      sockclient (--version)

    Options:
      -p <port>, --port <port>  Port the client sends to. [default: 1337].
      -i <ip>, --ip <ip>        IP address the client sends to [default: 127.0.0.1].
      -h, --help                Show this screen.
      --version                 Show version.
)";

int main(int argc, const char** argv)
{

    int port = 0;
    std::string ip_address = "";
    std::string version = "sockclient 0.1";
    /* client pointer that is used later */
    std::shared_ptr<sockclient::Client> client;
    /* set the title of the terminal window */
    rlutil::setConsoleTitle(version);
    /* docopt parses the arguments from the USAGE and creates a map from them */
    std::map<std::string, docopt::value> args
            = docopt::docopt(USAGE,
                             { argv + 1, argv + argc },
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
    } catch (const std::invalid_argument& e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is not a valid number and can not be a port." << std::endl;
        return -1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is too big to be a valid port." << std::endl;
        return -1;
    }

    ip_address = args["--ip"].asString();
    /* check if valid ip address was entered */
    if(!sockophil::Validator::is_valid_ip(ip_address)) {
        std::cerr << "Given ip address: " << ip_address << std::endl;
        std::cerr << "The argument is not a valid ip address." << std::endl;
        return -1;
    }
    try {
        /* create a new Client object */
        client = std::make_shared<sockclient::Client>(port, ip_address);
    } catch (const sockclient::CurrentDirectoryException& e) {
        std::cerr << "Could not resolve current directory. " << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const sockclient::SocketCreationException& e) {
        std::cerr << "Could not create the socket. " << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const sockclient::SocketConnectionException& e) {
        std::cerr << "Could not connect to the socket. " << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }
    /* run the client */
    client->run();
    return 0;
}