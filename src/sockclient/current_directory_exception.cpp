//
// Created by tobias on 01/10/16.
//

#include <cstring>
#include "sockclient/current_directory_exception.h"

namespace sockclient {
    CurrentDirectoryException::CurrentDirectoryException(int error_number) : error_number(error_number){
        this->error_text = std::strerror(error_number);
    }
    const char* CurrentDirectoryException::what() const noexcept {
        return error_text.c_str();
    }
}