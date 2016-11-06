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
 * @class AccessPackage AccessPackage.h "sockophil/AccessPackage.h"
 * @brief Package that is sent from the Server to the Client as response to login
 */
class AccessPackage : public Package {

  /**
   * @var tries stores the number of tries
   */
  unsigned short tries;

  /**
   * @var max_tries stores the maximum number of tries
   */
  unsigned short max_tries;

  /**
   * @var ok is set to true if login was ok else false
   */
  bool ok;

 public:
  /* need a default constructor for Cereal to work */
  AccessPackage() = default;

  AccessPackage(bool ok, unsigned short tries, unsigned short max_tries);

  unsigned short get_tries() const noexcept;

  unsigned short get_max_tries() const noexcept;

  bool get_ok() const noexcept;

  std::string get_type() const noexcept;

  template<class Archive>
  void serialize(Archive &ar);
};
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::AccessPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::AccessPackage)
