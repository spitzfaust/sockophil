//
// Created by tobias on 06/11/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/Constants.h"
#include "sockophil/LoginPackage.h"


namespace sockophil {
LoginPackage::LoginPackage(std::string username, std::string password) : username(username), password(password) {
}
std::string LoginPackage::get_type() const noexcept {
  return sockophil::LOGIN_PACKAGE;
}

std::string LoginPackage::get_username() const noexcept {
  return this->username;
}

std::string LoginPackage::get_password() const noexcept {
  return this->password;
}

template<class Archive>
void LoginPackage::serialize(Archive &ar) {
  ar(this->username, this->password);
}
}