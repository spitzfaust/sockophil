//
// Created by tobias on 01/10/16.
//

#include <unistd.h>
#include <errno.h>
#include <memory>

#include "sockclient/current_directory_exception.h"
#include "sockclient/client.h"

namespace sockclient {

    Client::Client(int port, std::string ip_address) : port(port), ip_address(ip_address) {
        char current_directory[1024];
        if(getcwd(current_directory, sizeof(current_directory)) == nullptr) {
            throw CurrentDirectoryException(errno);
        }
        this->current_directory = current_directory;
        this->menu = std::make_shared<Menu>(port, ip_address, current_directory);
    }
}
