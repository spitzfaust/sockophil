//
// Created by tobias on 01/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockclient {
    /**
     * @class CurrentDirectoryException CurrentDirectoryException.h "sockclient/CurrentDirectoryException.h"
     * @brief Is thrown if current directory can not be resolved
     */
    class CurrentDirectoryException : public sockophil::ErrnoException {
    public:
        CurrentDirectoryException(int error_number);
    };
}
