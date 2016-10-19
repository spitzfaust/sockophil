//
// Created by tobias on 01/10/16.
//

#include "sockclient/SocketConnectionException.h"

namespace sockclient {
    SocketConnectionException::SocketConnectionException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}