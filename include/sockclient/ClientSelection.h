//
// Created by tobias on 01/10/16.
//

#pragma once

#include "nlohmann/json.hpp"
#include "sockophil/protocol.h"

namespace sockclient {
    /**
     * @class ClientSelection ClientSelection.h "sockclient/ClientSelection.h"
     * @brief Container for the selection of the user
     */
    class ClientSelection {
        /**
         * @var action is the selected action
         */
        sockophil::client_action action;
        /**
         * @var filename can be the upload or download filename or empty
         */
        std::string filename;
    public:
        ClientSelection(sockophil::client_action action, std::string filename);
        ClientSelection(sockophil::client_action action);
        std::string get_filename() const noexcept;
        sockophil::client_action get_action() const noexcept;
        nlohmann::json to_json() const;
    };

}