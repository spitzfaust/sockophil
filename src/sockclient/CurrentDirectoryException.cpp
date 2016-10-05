//
// Created by tobias on 01/10/16.
//

#include "sockclient/CurrentDirectoryException.h"

namespace sockclient {
    CurrentDirectoryException::CurrentDirectoryException(int error_number)
            : sockophil::ErrnoException(error_number) {}
}