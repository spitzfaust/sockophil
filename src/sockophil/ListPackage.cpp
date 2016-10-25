//
// Created by tobias on 19/10/16.
//

#include "cereal/types/vector.hpp"
#include "sockophil/Constants.h"
#include "sockophil/ListPackage.h"

namespace sockophil {
ListPackage::ListPackage(std::vector<std::string> list) : list(list) {
}
std::string ListPackage::get_type() const noexcept {
  return sockophil::LIST_PACKAGE;
}
std::vector<std::string> ListPackage::get_list() const noexcept {
  return this->list;
}

template<class Archive>
void ListPackage::serialize(Archive &ar) {
  ar(this->list);
}
}