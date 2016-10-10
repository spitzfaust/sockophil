//
// Created by tobias on 09/10/16.
//

#include "cereal/types/vector.hpp"
#include "sockophil/constants.h"
#include "sockophil/DataPackage.h"

namespace sockophil {
    DataPackage::DataPackage(std::vector<uint8_t> data_raw) : data_raw(data_raw) {
    }

    std::vector<uint8_t> DataPackage::get_data_raw() const noexcept {
        return this->data_raw;
    }

    std::string DataPackage::get_type() const noexcept {
        return DATA_PACKAGE;
    }

    template<class Archive>
    void DataPackage::serialize(Archive &ar) {
        ar(this->data_raw);
    }

}