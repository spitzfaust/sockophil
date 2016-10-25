//
// Created by tobias on 05/10/16.
//

#pragma once

#include <exception>

namespace sockclient {
/**
 * @class NoFilenameException NoFilenameException.h "sockclient/NoFilenameException.h"
 * @brief Exception is thrown if a filename should be given
 */
class NoFilenameException : public std::exception {
 public:
  NoFilenameException();
  virtual const char *what() const noexcept;
};
}
