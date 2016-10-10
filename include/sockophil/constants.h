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
    const std::string START_REQUEST = "START_REQUEST";
    const std::string END_REQUEST = "END_REQUEST";
    const std::string START_DATA = "START_DATA";
    const std::string END_DATA = "END_DATA";


}