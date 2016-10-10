//
// Created by tobias on 01/10/16.
//

#include <cstring>
#include <sockophil/ErrnoException.h>
#include "sockclient/SocketCreationException.h"

namespace sockclient {
    SocketCreationException::SocketCreationException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}