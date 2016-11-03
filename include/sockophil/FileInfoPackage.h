//
// Created by tobias on 09/10/16.
//

#pragma once

#include <string>
#include <vector>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Package.h"

namespace sockophil {
/**
 * @class DataPackage DataPackage.h "sockophil/DataPackage.h"
 * @brief This package type is used for the transmission of files.
 */
class FileInfoPackage : public Package {
  /**
   * @var filename is the name of the file in the package
   */
  std::string filename;
 public:
  /* need a default constructor for Cereal to work */
  FileInfoPackage() = default;
  FileInfoPackage(std::string filename);
  std::string get_filename() const noexcept;
  std::string get_type() const noexcept;
  template<class Archive>
  void serialize(Archive &ar);
};
}
/* Register the serialisation type */
CEREAL_REGISTER_TYPE(sockophil::FileInfoPackage);
CEREAL_REGISTER_POLYMORPHIC_RELATION(sockophil::Package, sockophil::FileInfoPackage);
