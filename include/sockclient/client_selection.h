//
// Created by tobias on 01/10/16.
//

#pragma once

#include "sockophil/protocol.h"

namespace sockclient {

    class ClientSelection {
        sockophil::client_action action;
        std::string filename;
    public:
        ClientSelection(sockophil::client_action action, std::string filename);
        std::string get_filename();
        sockophil::client_action get_action();
    };

}