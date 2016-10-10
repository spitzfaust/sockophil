//
// Created by tobias on 01/10/16.
//

#include <cstring>
#include <sockophil/ErrnoException.h>
#include "sockophil/SocketCreationException.h"

namespace sockophil {
    SocketCreationException::SocketCreationException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}