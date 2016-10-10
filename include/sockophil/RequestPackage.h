//
// Created by tobias on 09/10/16.
//



#include <string>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"
#include "sockophil/protocol.h"

namespace sockophil {
    class RequestPackage : public Package {
        /**
         * @var action to perform
         */
        sockophil::client_action action;
        /**
         * @var filename can be the upload or download filename or empty
         */
        std::string filename;
        /**
         * @var filesize is the size of the file or empty
         */
        unsigned long filesize;
    public:
        RequestPackage() = default;
        RequestPackage(sockophil::client_action action, std::string filename, unsigned long filesize);

        RequestPackage(sockophil::client_action action, std::string filename);

        RequestPackage(sockophil::client_action action);

        std::string get_filename() const noexcept;

        sockophil::client_action get_action() const noexcept;

        unsigned long get_filesize() const noexcept;

        std::string get_type() const noexcept;

        template<class Archive>
        void serialize(Archive &ar);
    };
}
CEREAL_REGISTER_TYPE(sockophil::RequestPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::RequestPackage)
