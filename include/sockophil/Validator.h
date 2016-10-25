//
// Created by tobias on 05/10/16.
//

#pragma once

#include <string>

namespace sockophil {
/**
 * @class Validator validator.h "sockophil/validator.h"
 * @brief Class that holds static methods to validate values
 */
class Validator {
 public:
  static bool is_valid_ip(const std::string &ip_address);
  static bool is_valid_port(const int &port);
};
}