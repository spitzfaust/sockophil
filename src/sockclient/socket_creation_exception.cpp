//
// Created by tobias on 01/10/16.
//

#include <cstring>
#include "sockclient/socket_creation_exception.h"

namespace sockclient {
    SocketCreationException::SocketCreationException(int error_number) : error_number(error_number){
        this->error_text = std::strerror(error_number);
    }
    const char* SocketCreationException::what() const noexcept {
        return error_text.c_str();
    }
}