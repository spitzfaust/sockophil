//
// Created by tobias on 09/10/16.
//

#pragma once

#include <string>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"
#include "sockophil/Protocol.h"

namespace sockophil {
    class RequestPackage : public Package {
        /**
         * @var action to perform
         */
        sockophil::ClientAction action;
        /**
         * @var filename can be the upload or download filename or empty
         */
        std::string filename;

    public:
        RequestPackage() = default;

        RequestPackage(sockophil::ClientAction action, std::string filename);

        RequestPackage(sockophil::ClientAction action);

        std::string get_filename() const noexcept;

        sockophil::ClientAction get_action() const noexcept;

        std::string get_type() const noexcept;

        template<class Archive>
        void serialize(Archive &ar);
    };
}
CEREAL_REGISTER_TYPE(sockophil::RequestPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::RequestPackage)
