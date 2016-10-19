//
// Created by Pommi on 9/30/16.
//
#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>

namespace sockserver {
    class Server {
    private:
        unsigned short port;
        int socket_descriptor;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        std::string target_directory;
        //std::shared_ptr<Menu> menu;

        void create_socket();

        void bind_to_socket();

        void listen_on_socket();

        void close_socket();

        unsigned int number_digits(long number);

    public:
        Server(unsigned short port, std::string target_dir);

        ~Server();

        void run();
    };

}