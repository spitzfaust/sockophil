//
// Created by tobias on 20/10/16.
//

#pragma once

#include "sockophil/ErrnoException.h"

namespace sockophil {
    class SocketReceiveException : public ErrnoException {
    public:
        SocketReceiveException(int error_number);
    };
}