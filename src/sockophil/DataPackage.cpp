//
// Created by tobias on 09/10/16.
//

#include "cppcodec/base64_default_rfc4648.hpp"
#include "sockophil/constants.h"

#include "sockophil/DataPackage.h"

namespace sockophil {
    DataPackage::DataPackage(std::vector<uint8_t> data_raw) : data_raw(data_raw) {
        this->data_base64 = base64::encode(data_raw);
    }

    DataPackage::DataPackage(std::string data_base64) : data_base64(data_base64) {
        this->data_raw = base64::decode(data_base64);
    }

    std::string DataPackage::get_data_base64() const noexcept {
        return this->data_base64;
    }

    std::vector<uint8_t> DataPackage::get_data_raw() const noexcept {
        return this->data_raw;
    }

    nlohmann::json DataPackage::to_json() const noexcept {
        nlohmann::json jsn = {
                {"data", this->get_data_base64()}
        };
        return jsn;
    }

    std::string DataPackage::to_send_string() const noexcept {
        return START_DATA + this->to_json().dump() + END_DATA;
    }
}