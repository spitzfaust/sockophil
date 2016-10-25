/**
 * @file ErrnoExceptions.h
 * @brief This file holds all exception classes that have the ErrnoException as parent
 */
#pragma once
#include <exception>
#include <string>

namespace sockophil {
/**
 * @class ErrnoException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief This exception class is used as parent for C error exceptions with errno set.
 */
class ErrnoException : public std::exception {
 protected:
  /**
   * @var error_number is the errno of the Error
   */
  int error_number;
  /**
   * @var error_test is the text returned for the errno by std::strerror()
   */
  std::string error_text;
 public:
  ErrnoException(int error_number);
  virtual const char *what() const noexcept;
};

/**
 * @class SocketCreationException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if the creation of the socket was not successful
 */
class SocketCreationException : public sockophil::ErrnoException {
 public:
  SocketCreationException(int error_number);
};

/**
 * @class SocketReceiveException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if an error happend when receiving from a socket
 */
class SocketReceiveException : public ErrnoException {
 public:
  SocketReceiveException(int error_number);
};

/**
 * @class SocketConnectionException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is throwen if the connection to the socket could not be established
 */
class SocketConnectionException : public sockophil::ErrnoException {
 public:
  SocketConnectionException(int error_number);
};

/**
 * @class CurrentDirectoryException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if current directory can not be resolved
 */
class CurrentDirectoryException : public sockophil::ErrnoException {
 public:
  CurrentDirectoryException(int error_number);
};

/**
 * @class SocketAcceptException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if the binding of the socket was not successfull
 */
class SocketAcceptException : public sockophil::ErrnoException {
 public:
  SocketAcceptException(int error_number);
};

/**
 * @class SocketBindException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if the binding of the socket was not successfull
 */
class SocketBindException : public sockophil::ErrnoException {
 public:
  SocketBindException(int error_number);
};

/**
 * @class SocketListenException ErrnoExceptions.h "sockophil/ErrnoExceptions.h"
 * @brief Is thrown if the listen function returns an error
 */
class SocketListenException : public sockophil::ErrnoException {
 public:
  SocketListenException(int error_number);
};

}
