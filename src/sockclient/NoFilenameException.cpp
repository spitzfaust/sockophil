//
// Created by tobias on 05/10/16.
//

#include "sockclient/NoFilenameException.h"

namespace sockclient {
NoFilenameException::NoFilenameException() {}

const char *NoFilenameException::what() const noexcept {
  return "The selected action was PUT or GET but no valid filename was entered.";
}
}