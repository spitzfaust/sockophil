//
// Created by tobias on 09/10/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/constants.h"
#include "sockophil/RequestPackage.h"

namespace sockophil {
    RequestPackage::RequestPackage(sockophil::client_action action, std::string filename, unsigned long filesize)
            : action(action), filename(filename), filesize(filesize) {}

    RequestPackage::RequestPackage(sockophil::client_action action, std::string filename)
            : RequestPackage(action, filename, 0) {}

    RequestPackage::RequestPackage(sockophil::client_action action)
            : RequestPackage(action, "", 0) {}

    sockophil::client_action RequestPackage::get_action() const noexcept {
        return this->action;
    }

    std::string RequestPackage::get_filename() const noexcept {
        return this->filename;
    }

    unsigned long RequestPackage::get_filesize() const noexcept {
        return this->filesize;
    }

    std::string RequestPackage::get_type() const noexcept {
        return REQUEST_PACKAGE;
    }

    template<class Archive>
    void RequestPackage::serialize(Archive &ar) {
        ar(this->action, this->filename, this->filesize);
    }



}