//
// Created by tobias on 20/10/16.
//

#include "sockophil/SocketReceiveException.h"

namespace sockophil {
    SocketReceiveException::SocketReceiveException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}