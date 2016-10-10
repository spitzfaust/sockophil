//
// Created by tobias on 01/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockclient {
    /**
     * @class SocketConnectionException SocketConnectionException.h "sockclient/SocketConnectionException.h"
     * @brief Is throwen if the connection to the socket could not be established
     */
    class SocketConnectionException : public sockophil::ErrnoException {
    public:
        SocketConnectionException(int error_number);
    };
}
