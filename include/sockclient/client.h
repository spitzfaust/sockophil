//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>
#include "sockclient/menu.h"

namespace sockclient {

    class Client {
        unsigned short port;
        int socket_descriptor;
        struct sockaddr_in address;
        bool connected;
        std::string ip_address;
        std::string current_directory;
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