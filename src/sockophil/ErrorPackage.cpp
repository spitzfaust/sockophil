//
// Created by tobias on 20/10/16.
//

#include "sockophil/Constants.h"
#include "sockophil/ErrorPackage.h"

namespace sockophil {
ErrorPackage::ErrorPackage(ErrorCode error_code) : error_code(error_code) {}

std::string ErrorPackage::get_type() const noexcept {
  return ERROR_PACKAGE;
}

ErrorCode ErrorPackage::get_error_code() const noexcept {
  return this->error_code;
}

template<class Archive>
void ErrorPackage::serialize(Archive &ar) {
  ar(this->error_code);
}
}