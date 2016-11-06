//
// Created by tobias on 01/10/16.
//

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <memory>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include "sockophil/Networking.h"
#include "sockophil/Helper.h"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/Constants.h"
#include "sockophil/FileInfoPackage.h"
#include "sockophil/LoginPackage.h"
#include "sockophil/AccessPackage.h"
#include "sockophil/ActionPackage.h"
#include "sockophil/ErrnoExceptions.h"
#include "sockclient/Client.h"
#include "sockophil/Protocol.h"

namespace sockclient {

/**
 * @brief Constructor that sets the member vars, creates the socket and connects to the socket
 * @param port to connect to
 * @param ip_address to connect tom
 */
Client::Client(unsigned short port, std::string ip_address)
    : port(port), ip_address(ip_address), logged_in(false), blocked(false) {
  char current_directory[1024];
  /* get the current working directory string */
  if (getcwd(current_directory, sizeof(current_directory)) == nullptr) {
    throw sockophil::CurrentDirectoryException(errno);
  }
  this->current_directory = sockophil::Helper::add_trailing_slash(current_directory);
  this->menu = std::make_unique<Menu>(port, ip_address, current_directory);
  this->connected = false;
  this->create_socket();
  this->connect_to_socket();

}

/**
 * @brief Try to create a socket
 */
void Client::create_socket() {
  this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  /* socket function returned error code */
  if (this->socket_descriptor == -1) {
    throw sockophil::SocketCreationException(errno);
  }
  /* fill address with zeros */
  std::memset(&this->address, 0, sizeof(this->address));
  this->address.sin_family = AF_INET;
  this->address.sin_port = htons(this->port);
  inet_aton(this->ip_address.c_str(), &this->address.sin_addr);
}

/**
 * @brief Calls the menu to prompt the user for input and reacts on it.
 */
void Client::run() {
  bool keep_running = true;
  this->menu->render_login_info();
  while (!this->logged_in && !this->blocked) {
    this->login();
  }

  if (this->blocked) {
    return;
  }

  while (keep_running) {
    /* get a selection */
    auto selection = this->menu->selection_prompt();
    switch (selection.first) {
      case sockophil::LIST:
        this->request_a_list();
        break;
      case sockophil::QUIT:
        this->bid_server_farewell();
        keep_running = false;
        break;
      case sockophil::GET:
        this->download_a_file(selection.second);
        break;
      case sockophil::PUT:
        this->upload_a_file(selection.second);
        break;
    }
  }
}

/**
 * @brief Request a list of files from the server and let the menu render it
 */
void Client::request_a_list() {
  /* send the LIST request to the server */
  this->send_package(std::make_shared<sockophil::ActionPackage>(sockophil::LIST));
  /* hopefully receive a list package from the server */
  auto pkg = this->receive_package();
  if (pkg->get_type() == sockophil::LIST_PACKAGE) {
    /* cast the Package ptr to a ListPackage */
    auto list_pkg = std::static_pointer_cast<sockophil::ListPackage>(pkg);
    this->menu->render_list_response(list_pkg->get_list());
  } else if (pkg->get_type() == sockophil::ERROR_PACKAGE) {
    this->menu->render_error(std::static_pointer_cast<sockophil::ErrorPackage>(pkg)->get_error_code());
  } else {
    this->menu->render_error(sockophil::WRONG_PACKAGE);
  }
}

/**
 * @brief Send the QUIT action to the server and render a goodby message
 */
void Client::bid_server_farewell() {
  this->send_package(std::make_shared<sockophil::ActionPackage>(sockophil::QUIT));
  /** @todo maybe own goodby function */
  this->menu->render_success("Goodbye!");

}

/**
 * @brief Try to upload a file to the server
 * @param filepath is the path of the file to upload
 */
void Client::upload_a_file(std::string filepath) {
  /* should point to a ErrorPackage or SuccessPackage */
  std::shared_ptr<sockophil::Package> received_pkg = nullptr;
  /* filestream to read from */
  std::ifstream input_file;
  /* read from the file in binary mode */
  input_file.open(filepath, std::ios::in | std::ios::binary);
  /* check if the file could be opened */
  if (input_file.is_open()) {
    std::string filename = sockophil::Helper::parse_filename(filepath);
    /* send the request to upload the file */
    this->send_package(std::make_shared<sockophil::ActionPackage>(sockophil::PUT));
    /* send the actual data */
    this->send_package(std::make_shared<sockophil::FileInfoPackage>(filename));
    this->menu->render_status_upload();
    this->socket_send_file(this->socket_descriptor, input_file,
                           [this](const unsigned long &current, const unsigned long &total) {
                             this->menu->render_progress(current, total);
                           });
    /* wait for the success or error message */
    received_pkg = this->receive_package();
    if (received_pkg->get_type() == sockophil::SUCCESS_PACKAGE) {
      this->menu->render_success("Successfully uploaded the file " + filename + " to the server!");
    } else if (received_pkg->get_type() == sockophil::ERROR_PACKAGE) {
      this->menu->render_error(
          std::static_pointer_cast<sockophil::ErrorPackage>(received_pkg)
              ->get_error_code());
    } else {
      /* received a package that was not expected */
      this->menu->render_error(sockophil::WRONG_PACKAGE);
    }
  } else {
    this->menu->render_error("Put Error: File could not be opened!");
  }
  /* close the file that was read from */
  input_file.close();
}

/**
 * @brief Download a file from the server to the current folder
 * @param filename is the name of the file that should be downloaded
 */
void Client::download_a_file(std::string filename) {
  /* send a get request with the filename */
  this->send_package(std::make_shared<sockophil::ActionPackage>(sockophil::GET, filename));
  /* hopefully receive a FileInfoPackage from the server */
  auto received_pkg = this->receive_package();
  /* check if the received package is a FileInfoPackage */
  if (received_pkg->get_type() == sockophil::FILE_INFO_PACKAGE) {
    /* cast the package to a FileInfoPackage */
    auto file_info = std::static_pointer_cast<sockophil::FileInfoPackage>(received_pkg);
    /* write to this file */
    std::ofstream output_file;
    /* open the file in the current path in binary write mode */
    output_file.open(this->current_directory + file_info->get_filename(), std::ios::out | std::ios::binary);
    /* check if file could be opened */
    if (output_file.is_open()) {
      /* write the data to the file */
      this->socket_store_file(this->socket_descriptor,
                              output_file,
                              [this](const unsigned long &current, const unsigned long &total) {
                                this->menu->render_progress(current, total);
                              });
      this->menu->render_success(
          "Successfully downloaded the file " + file_info->get_filename() + " to the current folder!");
    } else {
      this->menu->render_error("Get Error: Could not store the file locally.");
    }
    /* close the file */
    output_file.close();
  } else if (received_pkg->get_type() == sockophil::ERROR_PACKAGE) {
    this->menu->render_error(std::static_pointer_cast<sockophil::ErrorPackage>(received_pkg)
                                 ->get_error_code());
  } else {
    this->menu->render_error(sockophil::WRONG_PACKAGE);
  }
}

void Client::login() {
  auto user_data = this->menu->login_prompt();
  this->send_package(std::make_shared<sockophil::LoginPackage>(user_data.first, user_data.second));
  auto received_pkg = this->receive_package();
  if (received_pkg->get_type() == sockophil::ACCESS_PACKAGE) {
    auto access_pkg = std::static_pointer_cast<sockophil::AccessPackage>(received_pkg);
    if (access_pkg->get_ok()) {
      this->logged_in = true;
    } else {
      this->menu->render_login_error(access_pkg->get_tries(), access_pkg->get_max_tries());
    }
  } else if (received_pkg->get_type() == sockophil::ERROR_PACKAGE) {
    auto error_pkg = std::static_pointer_cast<sockophil::ErrorPackage>(received_pkg);
    if (error_pkg->get_error_code() == sockophil::CLIENT_BLOCKED) {
      this->blocked = true;
    }
    this->menu->render_error(error_pkg->get_error_code());
  } else {
    this->menu->render_error(sockophil::WRONG_PACKAGE);
  }
}

/**
 * @brief Send a Package to the server
 * @param package is the Package that should be sent
 */
void Client::send_package(const std::shared_ptr<sockophil::Package> package) {
  /* call the send package of the parent */
  sockophil::Networking::send_package(this->socket_descriptor, package);
}

/**
 * @brief Try to connect to the socket
 */
void Client::connect_to_socket() {
  if (connect(this->socket_descriptor, (struct sockaddr *) &this->address, sizeof(this->address)) != 0) {
    throw sockophil::SocketConnectionException(errno);
  } else {
    this->connected = true;
  }
}

/**
 * @brief Receive a package from the
 * @return a shared pointer to a received Package
 */
std::shared_ptr<sockophil::Package> Client::receive_package() {
  /* call the function of the parent class */
  return sockophil::Networking::receive_package(this->socket_descriptor);
}
}
