//
// Created by tobias on 20/10/16.
//

#pragma once

#include <string>
#include <vector>
#include "sockophil/protocol.h"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"


namespace sockophil {
    class ErrorPackage : public Package {
        ErrorCode error_code;
    public:
        ErrorPackage() = default;
        ErrorPackage(ErrorCode error_code);
        ErrorCode get_error_code() const noexcept;
        std::string get_type() const noexcept;
        template<class Archive>
        void serialize(Archive &ar);
    };
}
CEREAL_REGISTER_TYPE(sockophil::ErrorPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::ErrorPackage);
