//
// Created by tobias on 02/10/16.
//

#include <string>
#include "sockclient/client_selection.h"

namespace sockclient {
    ClientSelection::ClientSelection(sockophil::client_action action, std::string filename) :
            action(action),
            filename(filename) {}
    sockophil::client_action ClientSelection::get_action() const noexcept {
        return this->action;
    }
    std::string ClientSelection::get_filename() const noexcept {
        return this->filename;
    }
}