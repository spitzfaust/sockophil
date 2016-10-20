//
// Created by Pommi on 9/30/16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory>
#include <cstring>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include "sockophil/Helper.h"
#include "sockophil/ListPackage.h"
#include "cereal/archives/portable_binary.hpp"
#include "sockserver/Server.h"
#include "sockophil/SocketCreationException.h"
#include "sockserver/SocketBindException.h"
#include "sockserver/SocketListenException.h"
#include "sockophil/Package.h"
#include "sockophil/DataPackage.h"
#include "sockophil/RequestPackage.h"
#include "sockophil/constants.h"


namespace sockserver {

    Server::Server(unsigned short port, std::string target_directory) : port(port) {
        this->target_directory = sockophil::Helper::add_trailing_slash(target_directory);
        this->dir_list();
        this->create_socket();
        this->bind_to_socket();
    }
    Server::~Server() {
        this->close_socket();
    }
    void Server::create_socket() {
        this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(this->socket_descriptor == -1) {
            throw sockophil::SocketCreationException(errno);
        }
        std::memset(&this->server_address, 0, sizeof(this->server_address));
        /* telling the server address all values will be IP addresses */
        this->server_address.sin_family = AF_INET;
        /* dont know yet */
        this->server_address.sin_addr.s_addr = INADDR_ANY;
        /* telling the server address the port number given to our object and
         * putting it through htons (host to network short) just in case
         */
        this->server_address.sin_port = htons(this->port);

    }

    void Server::bind_to_socket() {
        if (bind(this->socket_descriptor, (struct sockaddr *) &this->server_address, sizeof(server_address)) != 0) {
            throw SocketBindException(errno);
        }
    }

    void Server::listen_on_socket() {
        /* we listen at our socket
         * the integer is the number of clients we want to listen to at once
         */
        if(listen(this->socket_descriptor, 5) < 0) {
            throw SocketListenException(errno);
        }
        bool keep_listening = true;
        socklen_t client_address_length = sizeof(struct sockaddr_in);
        int accepted_socket = -1;
        while (keep_listening) {
            std::cout << "Waiting for clients..." << std::endl;
            /* we start listening */
            if(accepted_socket < 0) {
                accepted_socket = accept(this->socket_descriptor, (struct sockaddr *) &this->client_address,
                                         &client_address_length);
                if (accepted_socket < 0) {
                    //throw SocketAcceptException(errno);
                    break;
                } else {
                    std::cout << "Client connected from " << inet_ntoa(this->client_address.sin_addr) << ": "
                              << ntohs(this->client_address.sin_port) << std::endl;
                    // @todo maybe message to client that server is ready
                }
            }

            auto received_pkg = this->receive_package(accepted_socket);
            std::cout << received_pkg->get_type() << std::endl;
            if(received_pkg->get_type() == sockophil::REQUEST_PACKAGE) {
                switch(std::static_pointer_cast<sockophil::RequestPackage>(received_pkg)->get_action()) {
                    case sockophil::list:
                        this->send_package(accepted_socket, std::make_shared<sockophil::ListPackage>(this->dir_list()));
                        break;
                    case sockophil::put:
                        this->put_file(accepted_socket);
                        break;
                    case sockophil::quit:
                        close(accepted_socket);
                        keep_listening = false;
                        break;
                    default:
                        break;
                }
            } else {
                auto dta = std::static_pointer_cast<sockophil::DataPackage>(received_pkg)->get_data_raw();
                for (auto &&item : dta) {
                    //std::cout << item << std::endl;
                }
            }
        }
    }

    void Server::close_socket() {
        close(this->socket_descriptor);
    }

    void Server::run() {
        this->listen_on_socket();
    }

    std::vector<std::string> Server::dir_list() const {
        DIR *dirptr;
        bool check = true;
        struct dirent *direntry;
        std::vector<std::string> list;
        dirptr = opendir(this->target_directory.c_str());
        if (dirptr != NULL){
            while (check) {
                direntry = readdir(dirptr);
                if (direntry) {
                    list.push_back(std::string(direntry->d_name));
                } else {
                    check = false;
                }
            }
            std::sort(list.begin(), list.end());
            list.erase(list.begin(), list.begin() + 2);
        }
        closedir(dirptr);
        return list;
    }

    void Server::put_file(int accepted_socket) {
        auto received_pkg = this->receive_package(accepted_socket);
        if(received_pkg->get_type() == sockophil::DATA_PACKAGE) {
            std::cout << sockophil::DATA_PACKAGE << std::endl;
            auto data_pkg = std::static_pointer_cast<sockophil::DataPackage>(received_pkg);
            std::ofstream output_file;
            output_file.open(this->target_directory + data_pkg->get_filename(), std::ios::out | std::ios::binary);
            output_file.write((char*) data_pkg->get_data_raw().data(), data_pkg->get_data_raw().size());
            output_file.close();
        }
    }
}