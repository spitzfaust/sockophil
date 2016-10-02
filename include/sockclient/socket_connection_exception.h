//
// Created by tobias on 01/10/16.
//

#pragma once

#include <exception>
#include <string>

namespace sockclient {
    class SocketConnectionException : public std::exception {
        int error_number;
        std::string error_text;
    public:
        SocketConnectionException(int error_number);
        virtual const char* what() const noexcept;
    };
}
