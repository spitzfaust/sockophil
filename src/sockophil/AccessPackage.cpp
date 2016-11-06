//
// Created by tobias on 06/11/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/Constants.h"
#include "sockophil/AccessPackage.h"

namespace sockophil {

AccessPackage::AccessPackage(bool ok, unsigned short tries, unsigned short max_tries)
    : ok(ok), tries(tries), max_tries(max_tries) {}

bool AccessPackage::get_ok() const noexcept {
  return this->ok;
}

unsigned short AccessPackage::get_tries() const noexcept {
  return this->tries;
}

unsigned short AccessPackage::get_max_tries() const noexcept {
  return this->max_tries;
}

std::string AccessPackage::get_type() const noexcept {
  return ACCESS_PACKAGE;
}

template<class Archive>
void AccessPackage::serialize(Archive &ar) {
  ar(this->ok, this->tries, this->max_tries);
}

}