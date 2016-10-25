//
// Created by tobias on 25/10/16.
//

#include <cstring>
#include "sockophil/ErrnoExceptions.h"

namespace sockophil {
/**
 * Consturctor takes the errno as parameter and sets the member vars
 * @param error_number is the errno number of the Error
 */
ErrnoException::ErrnoException(int error_number) : error_number(error_number){
  this->error_text = std::strerror(error_number);
}
/**
 * Error text of the error
 * @return error text
 */
const char* ErrnoException::what() const noexcept {
  return error_text.c_str();
}

SocketCreationException::SocketCreationException(int error_number)
    : ErrnoException(error_number) {}

SocketReceiveException::SocketReceiveException(int error_number)
    : ErrnoException(error_number) {}

CurrentDirectoryException::CurrentDirectoryException(int error_number)
    : ErrnoException(error_number) {}

SocketConnectionException::SocketConnectionException(int error_number)
    : ErrnoException(error_number) {}

SocketAcceptException::SocketAcceptException(int error_number)
    : ErrnoException(error_number) {}

SocketBindException::SocketBindException(int error_number)
    : ErrnoException(error_number) {}

SocketListenException::SocketListenException(int error_number)
    : ErrnoException(error_number) {}

}