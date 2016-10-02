//
// Created by tobias on 01/10/16.
//

#include <cstring>
#include "sockclient/socket_connection_exception.h"

namespace sockclient {
    SocketConnectionException::SocketConnectionException(int error_number) : error_number(error_number){
        this->error_text = std::strerror(error_number);
    }
    const char* SocketConnectionException::what() const noexcept {
        return error_text.c_str();
    }
}