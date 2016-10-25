//
// Created by tobias on 20/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockophil {
    /**
     * @class SocketReceiveException SocketReceiveException.h "sockclient/SocketReceiveException.h"
     * @brief Is thrown if an error happend when receiving from a socket
     */
    class SocketReceiveException : public ErrnoException {
    public:
        SocketReceiveException(int error_number);
    };
}