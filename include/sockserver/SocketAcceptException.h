//
// Created by tobias on 22/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockserver {
    /**
     * @class SocketAcceptException SocketAcceptException.h "sockserver/SocketAcceptException.h"
     * @brief Is throwen if the binding of the socket was not successfull
     */
    class SocketAcceptException : public sockophil::ErrnoException {
    public:
        SocketAcceptException(int error_number);
    };
}
