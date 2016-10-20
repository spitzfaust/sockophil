//
// Created by tobias on 09/10/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/constants.h"
#include "sockophil/RequestPackage.h"

namespace sockophil {
    RequestPackage::RequestPackage(sockophil::client_action action, std::string filename) :
            action(action),
            filename(filename) {}

    RequestPackage::RequestPackage(sockophil::client_action action)
            : RequestPackage(action, "")  {}

    sockophil::client_action RequestPackage::get_action() const noexcept {
        return this->action;
    }
    std::string RequestPackage::get_filename() const noexcept {
        return this->filename;
    }

    std::string RequestPackage::get_type() const noexcept {
        return REQUEST_PACKAGE;
    }

    template<class Archive>
    void RequestPackage::serialize(Archive &ar) {
        ar(this->action, this->filename);
    }



}