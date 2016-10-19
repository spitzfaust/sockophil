//
// Created by tobias on 10/10/16.
//

#include "sockserver/SocketBindException.h"

namespace sockserver {
    SocketBindException::SocketBindException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}