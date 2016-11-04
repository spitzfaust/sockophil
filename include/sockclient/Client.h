//
// Created by tobias on 01/10/16.
//

#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>
#include "sockophil/Networking.h"
#include "sockclient/Menu.h"
#include "sockophil/Package.h"
#include "sockophil/ListPackage.h"
#include "sockophil/ActionPackage.h"
#include "sockophil/FileInfoPackage.h"

namespace sockclient {

/**
 * @class Client Client.h "sockclient/Client.h"
 * @brief Class that is used to interact with the server
 */
class Client : public sockophil::Networking {
  /**
   * @var port is the socket port
   */
  unsigned short port;
  /**
   * @var ip_address is the ip address to connect to
   */
  std::string ip_address;
  /**
   * @var socket_descriptor is the value returned by socket() to identify the socket
   */
  int socket_descriptor;
  /**
   * @var address is the socket address used by many functions
   */
  struct sockaddr_in address;
  /**
   * @var connected is set true when connection was successful
   */
  bool connected;
  /**
   * @var current_directory Holds the path of the current directory
   */
  std::string current_directory;
  /**
   * @var menu is used to interact with the user and get orders
   */
  std::unique_ptr<Menu> menu;
  void create_socket();
  void connect_to_socket();
  void request_a_list() const;
  void download_a_file(std::string filename) const;
  void upload_a_file(std::string filepath) const;
  void bid_server_farewell() const;
  void send_package(const std::shared_ptr<sockophil::Package> data) const;
  std::shared_ptr<sockophil::Package> receive_package() const;

 public:
  Client(unsigned short port, std::string ip_address);
  void run();
};

}