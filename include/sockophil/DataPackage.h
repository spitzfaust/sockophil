//
// Created by tobias on 09/10/16.
//

#pragma once

#include <string>
#include <vector>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"


namespace sockophil {
    class DataPackage : public Package {
        std::string filename;
        std::vector<uint8_t> data_raw;
    public:
        DataPackage() = default;
        DataPackage(std::vector<uint8_t> data_raw, std::string filename);
        std::vector<uint8_t> get_data_raw() const noexcept;
        std::string get_filename() const noexcept;
        std::string get_type() const noexcept;
        template<class Archive>
        void serialize(Archive &ar);
    };
}
CEREAL_REGISTER_TYPE(sockophil::DataPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::DataPackage);
