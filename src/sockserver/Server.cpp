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
        /* We bind to the socket */
        if (bind(this->socket_descriptor, (struct sockaddr *) &this->server_address, sizeof(server_address)) != 0) {
            /* if bind gives something other than 0 we throw an exception */
            throw SocketBindException(errno);
        }
    }

    void Server::listen_on_socket() {
        /* we listen at our socket
         * the integer is the number of clients we want to listen to at once
         */
        if(listen(this->socket_descriptor, 5) < 0) {
            /* if listen fails we get a SocketListenException */
            throw SocketListenException(errno);
        }
        /* Length of th clients address is stored to be laer used in accept() */
        socklen_t client_address_length = sizeof(struct sockaddr_in);
        /* Endless-loop starts  accepting data */
        while (true) {
            /* A new integer for taking up the accept */
            int accepted_socket = 0;
            /* Output message that server is ready and waiting for clients */
            std::cout << "Waiting for clients..." << std::endl;
            /* We start accepting accept() takes our socket descriptor of our server socket, a cast reference of the client address
             * and a reference to the length of the length of the client address */
            accepted_socket = accept(this->socket_descriptor, (struct sockaddr *) &this->client_address, &client_address_length);
            /* If the accept fails a 0 is stored n the accept_socket variable and an exception is thrown */
            if (accepted_socket < 0) {
                /* Exception pending */
                //throw SocketAcceptException(errno);
                break;
            } else {
                /* Else output message telling where the client is connecting from is given with ip and port */
                std::cout << "Client connected from " << inet_ntoa(this->client_address.sin_addr) << ": " << ntohs(this->client_address.sin_port) << std::endl;
                // @todo maybe message to client that server is ready
            }
            /* A char array as Buffer is created */
            char buffer[sockophil::BUF] = "";
            /* A Char vector  for the whole incoming package */
            std::vector<char> incoming;
            /* size takes up the result of our recv() which is the length of the message or -1 for error */
            ssize_t size = 0;
            /* size_of_incoming will take up the total numeber of bytes that will be sent */
            long size_of_incoming = 0;
            do {
                size = recv(accepted_socket, buffer, (sockophil::BUF - 1), 0);
                /* If size is greater than zero something was received */
                if (size > 0) {
                    /* The last received index of our buffer array is changed to "\0" */
                    buffer[size] = '\0';
                    /* If size_of_incoming is 0 nothing has been sent yet and we dont know how big the package will be */
                    if(size_of_incoming == 0){
                        /* incoming is cleared */
                        incoming.clear();
                        std::stringstream ss;
                        /* buffer is read into ss */
                        ss << buffer;
                        /* and then into size_of_incoming so we know how many bytes will be sent */
                        ss >> size_of_incoming;
                        /* debug */
                        std::cout << "-- incoming " << size_of_incoming << std::endl;
                        std::cout << "-- 1 size " << size << std::endl;
                        unsigned int number_of_digets = this->number_digits(size_of_incoming) + 1;
                        /* if size is greater than number_of_digits more than just the size of the package was sent and
                         * we start reading right away (probably happens when first two sends are combined)*/
                        if(size > number_of_digets) {
                            /* debug */
                            std::cout << "-- 1 numd " << number_of_digets << std::endl;
                            std::cout << "-- 1 size " << size << std::endl;
                            /* now we read again */
                            for (unsigned long i = number_of_digets; i < size && size_of_incoming >= 0; ++i) {
                                /* debug */
                                std::cout << "-- 1 for soi: " << size_of_incoming << std::endl;
                                std::cout << "-- 1 for i:   " << i <<std::endl;
                                /* and push back onto our incoming */
                                incoming.push_back(buffer[i]);
                                std::cout << "-- 1 buf:     " << buffer[i] << std::endl;
                                /* as long as size_of_incoming is not 0 we reduce it by one to keep reading */
                                if(size_of_incoming > 0) {
                                    --size_of_incoming;
                                }
                            }
                            /* debug */
                            std::cout << "-- 1 soi " << size_of_incoming;
                        }
                    /* we already know the size of the package so we keep reading as normal */
                    } else {
                        for (unsigned long i = 0; i < size && size_of_incoming >= 0; ++i) {
                            /* debug */
                            std::cout << "-- 2 for soi: " << size_of_incoming << std::endl;
                            std::cout << "-- 2 for i:   " << i << std::endl;
                            /* and push back onto our incoming */
                            incoming.push_back(buffer[i]);
                            std::cout << "-- 2 buf:     " << buffer[i] << std::endl;
                            /* as long as size_of_incoming is not 0 we reduce it by one to keep reading */
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

}