//
// Created by tobias on 10/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockserver {
    /**
     * @class SocketBindException SocketBindException.h "sockserver/SocketBindException.h"
     * @brief Is throwen if the binding of the socket was not successfull
     */
    class SocketBindException : public sockophil::ErrnoException {
    public:
        SocketBindException(int error_number);
    };
}
