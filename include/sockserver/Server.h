//
// Created by Pommi on 9/30/16.
//
#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>
#include "sockophil/Networking.h"
#include "sockophil/Package.h"
#include "sockserver/ThreadPool.h"

namespace sockserver {
/**
 * @class Server Server.h "sockclient/Server.h"
 * @brief Class that is used to interact with the client
 */
class Server : public sockophil::Networking {
 private:
  /**
   * @var port the server listens on
   */
  unsigned short port;
  /**
   * @var socket_descriptor the server listens on
   */
  int socket_descriptor;
  /**
   * @var server_address is the address information of the server
   */
  struct sockaddr_in server_address;
  /**
   * @var target_directory is the directory the server stores files in
   */
  std::string target_directory;
  /**
   * @var pool is the ThreadPool that is used by the Server
   */
  std::unique_ptr<ThreadPool> pool;
  /**
   * @var mut is the mutex that is used to sync the tasks
   */
  std::mutex mut;

  void create_socket();

  void bind_to_socket();

  void listen_on_socket();

  void close_socket();

  void store_file(int accepted_socket);

  void return_file(int accepted_socket, std::string filename);

  std::vector<std::string> dir_list() const;

 public:
  bool LDAP_login(std::string username, std::string password);

  Server(unsigned short port, std::string target_dir);

  ~Server();

  void run();
};

}