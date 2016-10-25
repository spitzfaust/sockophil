//
// Created by tobias on 19/10/16.
//

#pragma once
#include <string>
#include <vector>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"

namespace sockophil {
/**
 * @class ListPackage ListPackage.h "sockophil/ListPackage.h"
 * @brief This package type is used to transmit the list of elements in the upload directory to the client.
 */
class ListPackage : public Package {
  /**
   * @var list of elements in the upload directory
   */
  std::vector<std::string> list;
 public:
  /* need a default constructor for Cereal to work */
  ListPackage() = default;
  ListPackage(std::vector<std::string> list);
  std::vector<std::string> get_list() const noexcept;
  std::string get_type() const noexcept;
  template<class Archive>
  void serialize(Archive &ar);
};
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::ListPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::ListPackage);