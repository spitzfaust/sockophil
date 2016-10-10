//
// Created by tobias on 10/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockserver {
    /**
     * @class SocketListenException SocketListenException.h "sockserver/SocketListenException.h"
     * @brief Is throwen if the listen function returns an error
     */
    class SocketListenException : public sockophil::ErrnoException {
    public:
        SocketListenException(int error_number);
    };
}
