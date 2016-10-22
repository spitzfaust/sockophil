//
// Created by tobias on 05/10/16.
//

#include <arpa/inet.h>

#include "sockophil/Validator.h"
#include "sockophil/Constants.h"

namespace sockophil {
    /**
     * Checks if the given string is a valid ip address
     * @param ip_address
     * @return true if valid
     */
    bool Validator::is_valid_ip(const std::string& ip_address) {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, ip_address.c_str(), &(sa.sin_addr));
        return result != 0;
    }

    /**
     * Check if the port is in the allowed port range
     * @param port
     * @return true if valid
     */
    bool Validator::is_valid_port(const int& port) {
        return !((port < MIN_PORT) || (port > MAX_PORT));
    }
}