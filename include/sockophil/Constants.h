//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>
namespace sockophil {

    const unsigned int MAX_PORT = 65535;
    const unsigned int MIN_PORT = 1024;
    const unsigned int DEFAULT_PORT = 1337;
    const unsigned int BUF = 1024;
    const std::string DATA_PACKAGE = "DATA_PACKAGE";
    const std::string REQUEST_PACKAGE = "REQUEST_PACKAGE";
    const std::string LIST_PACKAGE = "LIST_PACKAGE";
    const std::string ERROR_PACKAGE = "ERROR_PACKAGE";
    const std::string SUCCESS_PACKAGE = "SUCCESS_PACKAGE";
    const std::string SIZE_DELIM = "|";
    const unsigned int HEADER_SIZE = 22;



}