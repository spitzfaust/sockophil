//
// Created by tobias on 22/10/16.
//

#pragma once

#include <string>
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"

namespace sockophil {
/**
 * @class SuccessPackage SuccessPackage.h "sockophil/SuccessPackage.h"
 * @brief This package is sent when an action was successful
 */
class SuccessPackage : public Package {
  std::string filename;
 public:
  /* need a default constructor for Cereal to work */
  SuccessPackage() = default;
  SuccessPackage(std::string filename);

  std::string get_filename() const noexcept;
  std::string get_type() const noexcept;

  template<class Archive>
  void serialize(Archive &ar);
};
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::SuccessPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::SuccessPackage)
