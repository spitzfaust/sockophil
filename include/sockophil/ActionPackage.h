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
    /**
     * @class ActionPackage ActionPackage.h "sockophil/ActionPackage.h"
     * @brief Package that is sent from the Client to the Server to request some action
     */
    class ActionPackage : public Package {
        /**
         * @var action to perform
         */
        sockophil::ClientAction action;
        /**
         * @var filename can be the upload or download filename or empty
         */
        std::string filename;

    public:
        /* need a default constructor for Cereal to work */
        ActionPackage() = default;

        ActionPackage(sockophil::ClientAction action, std::string filename = "");

        std::string get_filename() const noexcept;

        sockophil::ClientAction get_action() const noexcept;

        std::string get_type() const noexcept;

        template<class Archive>
        void serialize(Archive &ar);
    };
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::ActionPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::ActionPackage)
