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

    Server::Server(unsigned short port, std::string target_directory) : port(port), target_directory(target_directory) {
        this->dir_list();
        this->create_socket();
        this->bind_to_socket();
        this->listen_on_socket();



        /* Write back a message to the sending client */
        int message = write(this->socket_descriptor, "I got your message", 18);

        if (message < 0) {
            /* write error*/
        };
        this->close_socket();
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
        socklen_t client_address_length = sizeof(struct sockaddr_in);
        while (true) {
            int accepted_socket = 0;
            std::cout << "Waiting for clients..." << std::endl;
            /* we start listening */
            accepted_socket = accept(this->socket_descriptor, (struct sockaddr *) &this->client_address, &client_address_length);
            if (accepted_socket < 0) {
                //throw SocketAcceptException(errno);
                break;
            } else {
                std::cout << "Client connected from " << inet_ntoa(this->client_address.sin_addr) << ": " << ntohs(this->client_address.sin_port) << std::endl;
                // @todo maybe message to client that server is ready
            }
            char buffer[sockophil::BUF] = "";
            std::vector<char> incoming;
            ssize_t size = 0;
            long size_of_incoming = 0;
            do {
                size = recv(accepted_socket, buffer, (sockophil::BUF - 1), 0);
                if (size > 0) {
                    buffer[size] = '\0';
                    if(size_of_incoming == 0){
                        incoming.clear();
                        std::stringstream ss;
                        ss << buffer;
                        ss >> size_of_incoming;
                        std::cout << "-- incoming " << size_of_incoming << std::endl;
                        std::cout << "-- 1 size " << size << std::endl;
                        unsigned int number_of_digets = this->number_digits(size_of_incoming) + 1;

                        if(size > number_of_digets) {
                            std::cout << "-- 1 numd " << number_of_digets << std::endl;
                            std::cout << "-- 1 size " << size << std::endl;
                            for (unsigned long i = number_of_digets; i < size && size_of_incoming >= 0; ++i) {
                                std::cout << "-- 1 for soi: " << size_of_incoming << std::endl;
                                std::cout << "-- 1 for i:   " << i <<std::endl;
                                incoming.push_back(buffer[i]);
                                std::cout << "-- 1 buf:     " << buffer[i] << std::endl;
                                if(size_of_incoming > 0) {
                                    --size_of_incoming;
                                }
                            }
                            std::cout << "-- 1 soi " << size_of_incoming;
                        }
                    } else {
                        for (unsigned long i = 0; i < size && size_of_incoming >= 0; ++i) {
                            std::cout << "-- 2 for soi: " << size_of_incoming << std::endl;
                            std::cout << "-- 2 for i:   " << i << std::endl;
                            incoming.push_back(buffer[i]);
                            std::cout << "-- 2 buf:     " << buffer[i] << std::endl;
                            if(size_of_incoming > 0) {
                                --size_of_incoming;
                            }
                        }
                    }

                    if(size_of_incoming == 0) {
                        std::cout << "Size of incoming: " << incoming.size() << std::endl;
                        std::stringstream data_stream(std::string(incoming.begin(), incoming.end()));
                        {
                            cereal::PortableBinaryInputArchive iarchive(data_stream);
                            std::shared_ptr<sockophil::Package> pkg;
                            iarchive(pkg);
                            std::cout << "-- PACKAGE: " << pkg->get_type() << std::endl;
                            if(pkg->get_type() == sockophil::REQUEST_PACKAGE) {
                                std::cout << std::static_pointer_cast<sockophil::RequestPackage>(pkg)->get_action() << std::endl;
                            } else {
                                auto dta = std::static_pointer_cast<sockophil::DataPackage>(pkg)->get_data_raw();
                                for (auto &&item : dta) {
                                    //std::cout << item << std::endl;
                                }
                            }
                        }
                    }


                    /**/
                } else if (size == 0) {
                    std::cout << "Client closed remote socket" << std::endl;
                    break;
                } else {
                    //this->server_error("recv error");
                    std::terminate();
                }
            } while (true);
            close(accepted_socket);
            printf("Here is the message: %s\n", buffer);
        }
    }

    void Server::close_socket() {
        close(this->socket_descriptor);
    }

    void Server::run() {
        this->listen_on_socket();
    }

    unsigned int Server::number_digits(long number) {
        unsigned int digits = 0;
        if (number < 0) digits = 1;
        while (number) {
            number /= 10;
            digits++;
        }
        return digits;
    }
    std::string Server::dir_list() const {
        DIR *dirptr;
        bool check = true;
        struct dirent *direntry;
        std::vector<std::string> filenames;
        std::string list = "";
        dirptr = opendir(this->target_directory.c_str());
        if (dirptr != NULL){
            while (check) {
                direntry = readdir(dirptr);
                if (direntry) {
                    filenames.push_back(std::string(direntry->d_name));
                } else {
                    check = false;
                }
            }
            std::sort(filenames.begin(), filenames.end());
            for (auto &&filename : filenames) {
                list = list + filename + "\n";
            }
            std::cout << list;
        }
        closedir(dirptr);
        return list;
    }
}