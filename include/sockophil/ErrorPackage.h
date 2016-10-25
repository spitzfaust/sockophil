//
// Created by tobias on 20/10/16.
//

#pragma once

#include <string>
#include <vector>
#include "sockophil/Protocol.h"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"


namespace sockophil {
    /**
     * @class ErrorPackage ErrorPackage.h "sockophil/ErrorPackage.h"
     * @brief This package type is used to inform the counterpart of an error that has occoured.
     */
    class ErrorPackage : public Package {
        /**
         * @var error_code describes the error that happened
         */
        ErrorCode error_code;
    public:
        /* need a default constructor for Cereal to work */
        ErrorPackage() = default;
        ErrorPackage(ErrorCode error_code);
        ErrorCode get_error_code() const noexcept;
        std::string get_type() const noexcept;
        template<class Archive>
        void serialize(Archive &ar);
    };
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::ErrorPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::ErrorPackage);
