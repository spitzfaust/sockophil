//
// Created by tobias on 06/11/16.
//

#pragma once

#include <string>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/json.hpp"
#include "sockophil/Package.h"
#include "sockophil/Protocol.h"

namespace sockophil {
/**
 * @class LoginPackage LoginPackage.h "sockophil/LoginPackage.h"
 * @brief Package that is sent from the Client to the Server to login
 */
class LoginPackage : public Package {
  /**
   * @var username is the username used for login
   */
  std::string username;
  /**
   * @var password is the password used for login
   */
  std::string password;

 public:
  /* need a default constructor for Cereal to work */
  LoginPackage() = default;

  LoginPackage(std::string username, std::string password);

  std::string get_username() const noexcept;

  std::string get_password() const noexcept;

  std::string get_type() const noexcept;

  template<class Archive>
  void serialize(Archive &ar);
};
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::LoginPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::LoginPackage)
