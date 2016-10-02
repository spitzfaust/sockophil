//
// Created by tobias on 01/10/16.
//

#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <memory>
#include <cstring>
#include <arpa/inet.h>
#include <sockclient/socket_connection_exception.h>

#include "sockclient/socket_creation_exception.h"
#include "sockclient/current_directory_exception.h"
#include "sockclient/client.h"

namespace sockclient {

    Client::Client(unsigned short port, std::string ip_address) : port(port), ip_address(ip_address) {
        char current_directory[1024];
        if(getcwd(current_directory, sizeof(current_directory)) == nullptr) {
            throw CurrentDirectoryException(errno);
        }
        this->current_directory = current_directory;
        this->menu = std::make_shared<Menu>(port, ip_address, current_directory);
        this->create_socket();
        this->connect_to_socket();

    }
    Client::~Client() {
        this->close_socket();
    }


    void Client::create_socket() {
        this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(this->socket_descriptor == -1) {
            throw SocketCreationException(errno);
        }
        std::memset(&this->address,0,sizeof(this->address));
        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(this->port);
        inet_aton(this->ip_address.c_str(), &this->address.sin_addr);
    }


    void Client::run() {
        this->menu->get_option();
    }

    void Client::connect_to_socket() {
        if (connect(this->socket_descriptor, (struct sockaddr *) &this->address, sizeof (this->address)) != 0) {
            throw SocketConnectionException(errno);
        } else {
            this->connected = true;
        }
    }

    void Client::close_socket() {
        close(this->socket_descriptor);
    }
}
