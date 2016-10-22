//
// Created by tobias on 09/10/16.
//

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "sockophil/Constants.h"
#include "sockophil/DataPackage.h"

namespace sockophil {
    DataPackage::DataPackage(std::vector<uint8_t> data_raw, std::string filename) :
            data_raw(data_raw), filename(filename) {}

    std::vector<uint8_t> DataPackage::get_data_raw() const noexcept {
        return this->data_raw;
    }

    std::string DataPackage::get_filename() const noexcept {
        return this->filename;
    }

    std::string DataPackage::get_type() const noexcept {
        return DATA_PACKAGE;
    }

    template<class Archive>
    void DataPackage::serialize(Archive &ar) {
        ar(this->data_raw, this->filename);
    }

}