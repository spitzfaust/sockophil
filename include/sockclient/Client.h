//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>
#include "sockclient/Menu.h"

namespace sockclient {

    /**
     * @class Client client.h "sockclient/client.h"
     * @brief Class that is used to interact with the server
     */
    class Client {
        /**
         * @var port is the socket port
         */
        unsigned short port;
        /**
         * @var ip_address is the ip address to connect to
         */
        std::string ip_address;
        /**
         * @var socket_descriptor is the value returned by socket() to identify the socket
         */
        int socket_descriptor;
        /**
         * @var address is the socket address used by many functions
         */
        struct sockaddr_in address;
        /**
         * @var connected is set true when connection was successful
         */
        bool connected;
        /**
         * @var current_directory Holds the path of the current directory
         */
        std::string current_directory;
        /**
         * @var menu is used to interact with the user and get orders
         */
        std::shared_ptr<Menu> menu;
        void create_socket();
        void connect_to_socket();
        void close_socket();
    public:
        Client(unsigned short port, std::string ip_address);
        ~Client();
        void run();
    };

}