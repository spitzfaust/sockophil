//
// Created by tobias on 22/10/16.
//

#include "sockophil/Constants.h"
#include "sockophil/SuccessPackage.h"

namespace sockophil {
    std::string SuccessPackage::get_type() const noexcept {
        return SUCCESS_PACKAGE;
    }
    template <class Archive>
    void SuccessPackage::serialize(Archive &ar) {
    }
}