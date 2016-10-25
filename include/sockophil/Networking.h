//
// Created by tobias on 19/10/16.
//

#pragma once

#include <memory>
#include "sockophil/AllPackages.h"

namespace sockophil {
    /**
     * @class Networking Networking.h "sockophil/Networking.h"
     * @brief Is used as parent for Server and Client and holds functions to send and receive packages.
     */
    class Networking {
    protected:
        std::shared_ptr<Package> receive_package(int socket_descriptor) const;
        void send_package(int socket_descriptor, const std::shared_ptr<Package> package) const;
    };
}
