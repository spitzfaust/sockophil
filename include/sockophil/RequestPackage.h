//
// Created by tobias on 09/10/16.
//



#include <string>
#include "sockophil/protocol.h"
#include "nlohmann/json.hpp"

namespace sockophil {
    class RequestPackage {
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
        RequestPackage(sockophil::client_action action, std::string filename, unsigned long filesize);

        RequestPackage(sockophil::client_action action, std::string filename);

        RequestPackage(sockophil::client_action action);

        RequestPackage(nlohmann::json jsn);

        std::string get_filename() const noexcept;

        sockophil::client_action get_action() const noexcept;

        unsigned int get_filesize() const noexcept;

        nlohmann::json to_json() const;

        std::string to_send_string() const;
    };
}