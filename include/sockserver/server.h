//
// Created by Pommi on 9/30/16.
//
#pragma once

#include <string>
#include <memory>
#include <netinet/in.h>

namespace sockclient {
  class Server {
  private:
    unsigned short port;
    int server_socket;
    int new_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    std::string target_directory;
    //std::shared_ptr<Menu> menu;

    void create_socket();

    void server_bind();

    void server_listen();

    void close_socket();

    void server_error(std::string error);

  public:
    Server(unsigned short port, std::string target_dir);

    ~Server();

    void run();
  };

}