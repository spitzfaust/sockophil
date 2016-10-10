//
// Created by tobias on 01/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockclient {
    /**
     * @class SocketCreationException SocketCreationException.h "sockclient/SocketCreationException.h"
     * @brief Is throwen if the creation of the socket was not successful
     */
    class SocketCreationException : public sockophil::ErrnoException {
    public:
        SocketCreationException(int error_number);
    };
}
