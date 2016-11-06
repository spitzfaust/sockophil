//
// Created by tobias on 22/10/16.
//

#include "sockophil/Constants.h"
#include "sockophil/SuccessPackage.h"

namespace sockophil {
SuccessPackage::SuccessPackage(std::string filename) : filename(filename) {

}
std::string SuccessPackage::get_filename() const noexcept {
  return this->filename;
}
std::string SuccessPackage::get_type() const noexcept {
  return SUCCESS_PACKAGE;
}
template<class Archive>
void SuccessPackage::serialize(Archive &ar) {
  ar(this->filename);
}
}