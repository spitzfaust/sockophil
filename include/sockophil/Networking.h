//
// Created by tobias on 19/10/16.
//

#pragma once

#include <memory>
#include <mutex>
#include "sockophil/AllPackages.h"

namespace sockophil {
/**
 * @class Networking Networking.h "sockophil/Networking.h"
 * @brief Is used as parent for Server and Client and holds functions to send and receive packages.
 */
class Networking {
 protected:
  /**
   * @var mut is the mutex that is used to sync the tasks
   */
  std::mutex mut;
  std::shared_ptr<Package> receive_package(int socket_descriptor);
  void send_package(int socket_descriptor, const std::shared_ptr<Package> package);
  long receive_header(const int &socket_descriptor) const;
  void send_header(const int &socket_descriptor, long header) const;
  void socket_store_file(const int &socket_descriptor, std::ofstream &output_file,
                         const std::function<void(const long &,
                                                  const long &)> &call) const;
  void socket_store_file(const int &socket_descriptor, std::ofstream &output_file) const;
  void socket_send_file(const int &socket_descriptor, std::ifstream &input_file,
                        const std::function<void(const long &,
                                                 const long &)> &call) const;
  void socket_send_file(const int &socket_descriptor, std::ifstream &input_file) const;
 public:
  Networking() = default;
  virtual ~Networking();
};
}
