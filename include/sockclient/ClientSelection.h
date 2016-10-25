//
// Created by tobias on 01/10/16.
//

#pragma once

#include "sockophil/Protocol.h"

namespace sockclient {
    /**
     * @class ClientSelection ClientSelection.h "sockclient/ClientSelection.h"
     * @brief Container for the selection of the user
     */
    class ClientSelection {
        /**
         * @var action is the selected action
         */
        sockophil::ClientAction action;
        /**
         * @var filename can be the upload or download filename or empty
         */
        std::string filename;
    public:
        ClientSelection(sockophil::ClientAction action, std::string filename = "");
        std::string get_filename() const noexcept;
        sockophil::ClientAction get_action() const noexcept;
    };

}