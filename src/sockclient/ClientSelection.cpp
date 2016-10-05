//
// Created by tobias on 02/10/16.
//

#include <string>
#include "sockclient/ClientSelection.h"

namespace sockclient {
    /**
     * Constructor that sets the member vars
     * @param action is the action that should be executed
     * @param filename is either the filename of the file to download or upload or empty (quit, list)
     */
    ClientSelection::ClientSelection(sockophil::client_action action, std::string filename) :
            action(action),
            filename(filename) {}

    /**
     * Get the action
     * @return action
     */
    sockophil::client_action ClientSelection::get_action() const noexcept {
        return this->action;
    }

    /**
     * Get the filename
     * @return filename
     */
    std::string ClientSelection::get_filename() const noexcept {
        return this->filename;
    }
}