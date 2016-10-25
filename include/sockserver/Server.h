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
    /**
     * @class Server Server.h "sockclient/Server.h"
     * @brief Class that is used to interact with the client
     */
    class Server : public sockophil::Networking {
    private:
        /**
         * @var port the server listens on
         */
        unsigned short port;
        /**
         * @var socket_descriptor the server listens on
         */
        int socket_descriptor;
        /**
         * @var server_address is the address information of the server
         */
        struct sockaddr_in server_address;
        /**
         * @var client_address is the address information of the connected client
         */
        struct sockaddr_in client_address;
        std::string target_directory;

        void create_socket();

        void bind_to_socket();

        void listen_on_socket();

        void close_socket();

        void store_file(int accepted_socket);

        void return_file(int accepted_socket, std::string filename);

        std::vector<std::string> dir_list() const;

    public:
        Server(unsigned short port, std::string target_dir);

        ~Server();

        void run();
    };

}