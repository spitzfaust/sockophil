//
// Created by tobias on 09/10/16.
//

#include "sockophil/constants.h"
#include "sockophil/RequestPackage.h"

namespace sockophil {
    RequestPackage::RequestPackage(sockophil::client_action action, std::string filename, unsigned long filesize)
            : action(action), filename(filename), filesize(filesize) {}

    RequestPackage::RequestPackage(sockophil::client_action action, std::string filename)
            : RequestPackage(action, filename, 0) {}

    RequestPackage::RequestPackage(sockophil::client_action action)
            : RequestPackage(action, "", 0) {}

    RequestPackage::RequestPackage(nlohmann::json jsn) {
        int action = jsn["action"];
        this->action = static_cast<sockophil::client_action>(action);
        this->filename = jsn["filename"];
        this->filesize = jsn["filesize"];
    }

    sockophil::client_action RequestPackage::get_action() const noexcept {
        return this->action;
    }

    std::string RequestPackage::get_filename() const noexcept {
        return this->filename;
    }

    unsigned int RequestPackage::get_filesize() const noexcept {
        return this->filesize;
    }

    nlohmann::json RequestPackage::to_json() const {
        nlohmann::json jsn = {
                {"action", this->get_action()},
                {"filename", this->get_filename()},
                {"filesize", this->get_filesize()}
        };
        return jsn;
    }

    std::string RequestPackage::to_send_string() const {
        return START_REQUEST + to_json().dump() + END_REQUEST;
    }



}