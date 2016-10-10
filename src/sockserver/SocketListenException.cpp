//
// Created by tobias on 10/10/16.
//

#include "sockserver/SocketListenException.h"

namespace sockserver {
    SocketListenException::SocketListenException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}