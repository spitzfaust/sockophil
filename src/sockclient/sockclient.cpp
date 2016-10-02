//
// Created by tobias on 29/09/16.
//

#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory>
#include <sockclient/client.h>
#include <sockclient/current_directory_exception.h>
#include "rlutil/rlutil.h"
#include "docopt.h"

#include "sockophil/constants.h"

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

bool is_valid_ip_address(const std::string &ip_address);

int main(int argc, const char** argv)
{
    int port = 0;
    std::string ip_address = "";
    std::shared_ptr<sockclient::Client> client;
    std::map<std::string, docopt::value> args
            = docopt::docopt(USAGE,
                             { argv + 1, argv + argc },
                             true,               // show help if requested
                             "sockclient 0.1");  // version string

    for(auto const& arg : args) {
        std::cout << arg.first << "  " <<  arg.second << std::endl;
    }
    try {
        port = std::stoi(args["--port"].asString());
        if (port < sockophil::MIN_PORT) {
            std::cerr << "Given port: " << args["--port"].asString() << std::endl;
            std::cerr << "The port has to be bigger than " << sockophil::MIN_PORT << "." << std::endl;
            return EXIT_FAILURE;
        } else if (port > sockophil::MAX_PORT) {
            std::cerr << "Given port: " << args["--port"].asString() << std::endl;
            std::cerr << "The port has to be smaller than " << sockophil::MAX_PORT << "." << std::endl;
            return EXIT_FAILURE;
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is not a valid number and can not be a port." << std::endl;
        return EXIT_FAILURE;
    } catch (const std::out_of_range& e) {
        std::cerr << "Given port: " << args["--port"].asString() << std::endl;
        std::cerr << "The argument is too big to be a valid port." << std::endl;
        return EXIT_FAILURE;
    }
    ip_address = args["--ip"].asString();
    if(!is_valid_ip_address(ip_address)) {
        std::cerr << "Given ip address: " << ip_address << std::endl;
        std::cerr << "The argument is not a valid ip address." << std::endl;
        return EXIT_FAILURE;
    }
    try {
        client = std::make_shared<sockclient::Client>(port, ip_address);

    } catch (const sockclient::CurrentDirectoryException& e) {
        std::cerr << "Could not resolve current directory. " << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    client->run();

    return EXIT_SUCCESS;
}

bool is_valid_ip_address(const std::string& ip_address)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address.c_str(), &(sa.sin_addr));
    return result != 0;
}