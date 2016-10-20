//
// Created by tobias on 19/10/16.
//

#pragma once
#include <string>
#include <vector>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"

namespace sockophil {
    class ListPackage : public Package {
        std::vector<std::string> list;
    public:
        ListPackage() = default;
        ListPackage(std::vector<std::string> list);
        std::vector<std::string> get_list() const noexcept;
        std::string get_type() const noexcept;
        template<class Archive>
        void serialize(Archive &ar);
    };
}

CEREAL_REGISTER_TYPE(sockophil::ListPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::ListPackage);