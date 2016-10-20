//
// Created by Pommi on 9/30/16.
//
#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>
#include "sockophil/Networking.h"
#include "sockophil/Package.h"

namespace sockserver {
    class Server : public sockophil::Networking {
    private:
        unsigned short port;
        int socket_descriptor;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        std::string target_directory;

        void create_socket();

        void bind_to_socket();

        void listen_on_socket();

        void close_socket();

        void put_file(int accepted_socket);

        void return_file(int accepted_socket, std::string filename);

        std::vector<std::string> dir_list() const;

    public:
        Server(unsigned short port, std::string target_dir);

        ~Server();

        void run();
    };

}