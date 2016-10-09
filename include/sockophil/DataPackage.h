//
// Created by tobias on 09/10/16.
//

#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

namespace sockophil {
    class DataPackage {
        /** @todo should maybe be a vector<char> */
        std::string data_base64;
        std::vector<uint8_t> data_raw;
    public:
        DataPackage(std::vector<uint8_t> data_raw);
        DataPackage(std::string data_base64);
        std::string get_data_base64() const noexcept;
        std::vector<uint8_t> get_data_raw() const noexcept;
        nlohmann::json to_json() const noexcept;
        std::string to_send_string() const noexcept;
    };
}
