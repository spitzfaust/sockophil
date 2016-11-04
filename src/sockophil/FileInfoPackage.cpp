#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "sockophil/Constants.h"
#include "sockophil/FileInfoPackage.h"

namespace sockophil {
FileInfoPackage::FileInfoPackage(std::string filename) :
     filename(filename) {}

std::string FileInfoPackage::get_filename() const noexcept {
  return this->filename;
}

std::string FileInfoPackage::get_type() const noexcept {
  return FILE_INFO_PACKAGE;
}

template<class Archive>
void FileInfoPackage::serialize(Archive &ar) {
  ar(this->filename);
}

}