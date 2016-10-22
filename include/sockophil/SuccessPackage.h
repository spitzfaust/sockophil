//
// Created by tobias on 22/10/16.
//

#pragma once

#include <string>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"

namespace sockophil {
    class SuccessPackage : public Package {

    public:
        SuccessPackage() = default;

        std::string get_type() const noexcept;

        template<class Archive>
        void serialize(Archive &ar);
    };
}
CEREAL_REGISTER_TYPE(sockophil::SuccessPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::SuccessPackage)
