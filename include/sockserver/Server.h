//
// Created by Pommi on 9/30/16.
//
#pragma once

#include <string>
#include <chrono>
#include <memory>
#include <netinet/in.h>
#include "sockophil/Networking.h"
#include "sockophil/Package.h"
#include "sockserver/ThreadPool.h"

namespace sockserver {
/**
 * @class Server Server.h "sockserver/Server.h"
 * @brief Class that is used to interact with the client
 */
class Server : public sockophil::Networking {
 private:
  /**
    * @var MAX_LOGIN_TRIES is the maximum number of login tries from a client on the server
    */
  const unsigned short MAX_LOGIN_TRIES = 3;
  /**
   * @var BLOCKING_TIME is the time a client is blocked
   */
  const std::chrono::minutes BLOCKING_MINUTES = std::chrono::minutes(3);
  /**
   * @var auth if true ldap auth is checked
   */
   bool auth;
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
   * @var file_muts is a map that holds a mutex for every file
   */
  std::map<std::string, std::unique_ptr<std::mutex>> file_muts;
  /**
   * @var blocked_clients is a map that holds the ip addresses and a timestamp of blocked clients
   */
  std::map<std::string, std::chrono::system_clock::time_point> blocked_clients;

  void create_socket();

  void bind_to_socket();

  void listen_on_socket();

  void close_socket();

  void store_file(int accepted_socket);

  void return_file(int accepted_socket, std::string filename);

  bool is_client_blocked(std::string ip);

  std::vector<std::string> directory_list();

  void add_file_mutex(std::string filename);

  void remove_file_mutex(std::string filename);

  bool ldap_login(std::string username, std::string password);

 public:

  Server(unsigned short port, std::string target_dir, bool auth);

  ~Server();

  void run();
};

}