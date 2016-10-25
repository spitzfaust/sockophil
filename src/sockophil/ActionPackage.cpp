//
// Created by tobias on 09/10/16.
//

#include "cereal/types/string.hpp"
#include "sockophil/Constants.h"
#include "sockophil/ActionPackage.h"

namespace sockophil {
ActionPackage::ActionPackage(sockophil::ClientAction action, std::string filename) :
    action(action),
    filename(filename) {}

sockophil::ClientAction ActionPackage::get_action() const noexcept {
  return this->action;
}
std::string ActionPackage::get_filename() const noexcept {
  return this->filename;
}

std::string ActionPackage::get_type() const noexcept {
  return ACTION_PACKAGE;
}

template<class Archive>
void ActionPackage::serialize(Archive &ar) {
  ar(this->action, this->filename);
}

}