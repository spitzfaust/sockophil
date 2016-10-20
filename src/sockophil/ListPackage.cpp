//
// Created by tobias on 19/10/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/constants.h"
#include "sockophil/ListPackage.h"

namespace sockophil {
    ListPackage::ListPackage(std::string list) : list(list){
    }
    std::string ListPackage::get_type() const noexcept {
        return sockophil::LIST_PACKAGE;
    }
    std::string ListPackage::get_list() const noexcept {
        return this->list;
    }

    template<class Archive>
    void ListPackage::serialize(Archive &ar) {
        ar(this->list);
    }
}