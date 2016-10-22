//
// Created by tobias on 22/10/16.
//

#include "sockserver/SocketAcceptException.h"

namespace sockserver {
    SocketAcceptException::SocketAcceptException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}