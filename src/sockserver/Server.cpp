#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory>
#include <cstring>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include <condition_variable>
#include <atomic>
#include "sockserver/Server.h"
#include "sockophil/Helper.h"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/ErrnoExceptions.h"
#include "sockophil/ListPackage.h"
#include "sockophil/Package.h"
#include "sockophil/DataPackage.h"
#include "sockophil/ActionPackage.h"
#include "sockophil/ErrorPackage.h"
#include "sockophil/Constants.h"
#include "ldap.h"
#include <termios.h>
#include <unistd.h>




namespace sockserver {

/**
 * @brief Constructor that stores member vars and sets up the socket
 * @param port is the port that should be listend on
 * @param target_directory is the directory to upload files
 */
Server::Server(unsigned short port, std::string target_directory) : port(port) {
  this->pool = std::make_unique<ThreadPool>();
  this->target_directory = sockophil::Helper::add_trailing_slash(target_directory);
  this->dir_list();
  this->create_socket();
  this->bind_to_socket();
}
/**
 * @brief Destructor closes the socket
 */
Server::~Server() {
  this->close_socket();
}

/**
 * @brief Try to create a socket
 */
void Server::create_socket() {
  this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socket_descriptor == -1) {
    throw sockophil::SocketCreationException(errno);
  }
  /* set the whole struct to 0's */
  std::memset(&this->server_address, 0, sizeof(this->server_address));
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = INADDR_ANY;
  this->server_address.sin_port = htons(this->port);
}

/**
 * @brief Bind to the socket
 */
void Server::bind_to_socket() {
  if (bind(this->socket_descriptor, (struct sockaddr *) &this->server_address, sizeof(server_address)) != 0) {
    throw sockophil::SocketBindException(errno);
  }
}

/**
 * @brief Listen to the socket in an endless loop
 */
void Server::listen_on_socket() {
  socklen_t client_address_length = sizeof(struct sockaddr_in);
  struct sockaddr_in client_address;
  /* try to start listening */
  if (listen(this->socket_descriptor, 5) < 0) {
    throw sockophil::SocketListenException(errno);
  }
  while (true) {
    int accepted_socket = -1;
    std::cout << "Waiting for clients..." << std::endl;
    /* should a new client be accepted */

    accepted_socket = accept(this->socket_descriptor, (struct sockaddr *) &client_address,
                             &client_address_length);
    /* check if an error happened while accepting */
    if (accepted_socket < 0) {
      throw sockophil::SocketAcceptException(errno);
    } else {
      std::cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << ": "
                << ntohs(client_address.sin_port) << std::endl;
      this->pool->schedule([this, accepted_socket](const std::atomic_bool &stop) {
        while (true) {
          if (stop) {
            /** @todo maybe msg to client to inform about shutdown */
            close(accepted_socket);
            return;
          }
          /* package that was sent by the client to the server */
          auto received_pkg = this->receive_package(accepted_socket);
          /* a ActionPackage should be received */
          if (received_pkg->get_type() == sockophil::ACTION_PACKAGE) {
            /* check which action should be performed */
            switch (std::static_pointer_cast<sockophil::ActionPackage>(received_pkg)->get_action()) {
              case sockophil::LIST:
                this->send_package(accepted_socket, std::make_shared<sockophil::ListPackage>(this->dir_list()));
                break;
              case sockophil::PUT:
                this->store_file(accepted_socket);
                break;
              case sockophil::GET:
                this->return_file(accepted_socket,
                                  std::static_pointer_cast<sockophil::ActionPackage>(received_pkg)
                                      ->get_filename());
                break;
              case sockophil::QUIT:
                close(accepted_socket);
                return;
            }
          }
        }
      });
    }
  }
}

/**
 * @brief Close the server socket
 */
void Server::close_socket() {
  close(this->socket_descriptor);
}

/**
 * @brief Run the server
 */
void Server::run() {
  this->listen_on_socket();
}
/**
 * @brief get a list of elements in the upload dir
 * @return a vector of elements in the upload dir
 */
std::vector<std::string> Server::dir_list() const {
  DIR *dirptr;
  struct dirent *direntry;
  std::vector<std::string> list;
  dirptr = opendir(this->target_directory.c_str());
  if (dirptr != NULL) {
    /* store every entry in the vector */
    while (true) {
      direntry = readdir(dirptr);
      if (direntry) {
        list.push_back(std::string(direntry->d_name));
      } else {
        break;
      }
    }
    /* sort the vector alphabetically */
    std::sort(list.begin(), list.end());
    /* erase first 2 entries (".","..") */
    list.erase(list.begin(), list.begin() + 2);
  }
  closedir(dirptr);
  return list;
}

/**
 * @brief Store a received file in the upload directory
 * @param accepted_socket is the socket to listen for the data
 */
void Server::store_file(int accepted_socket) {
  /* try to receive a DataPackage */
  auto received_pkg = this->receive_package(accepted_socket);
  /* Package for the response */
  std::shared_ptr<sockophil::Package> response_package = nullptr;
  /* should be a DataPackage */
  if (received_pkg->get_type() == sockophil::DATA_PACKAGE) {
    /* cast to a DataPackage */
    auto data_pkg = std::static_pointer_cast<sockophil::DataPackage>(received_pkg);
    /* file to write to */
    std::ofstream output_file;
    {
      std::lock_guard<std::mutex> lock(this->mut);
      /* try to open the file */
      output_file.open(this->target_directory + data_pkg->get_filename(), std::ios::out | std::ios::binary);
      /* check if file could be opened */
      if (output_file.is_open()) {
        /* write to the file and create SuccessPackage */

        output_file.write((char *) data_pkg->get_data_raw().data(), data_pkg->get_data_raw().size());
        response_package = std::make_shared<sockophil::SuccessPackage>();
      } else {
        /* file could not be stored */
        response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::FILE_STORAGE);
      }
      output_file.close();
    }
  } else {
    /* was not the expected DataPackage */
    response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::WRONG_PACKAGE);
  }
  /* send the response to the client */
  this->send_package(accepted_socket, response_package);
}

/**
 * @brief Send a requested file to the client
 * @param accepted_socket is the socket that is used to communicate over
 * @param filename is the requested file
 */
void Server::return_file(int accepted_socket, std::string filename) {
  std::shared_ptr<sockophil::Package> response_package = nullptr;
  std::string filepath = this->target_directory + filename;
  /* data of the file */
  std::vector<uint8_t> file_data;
  /* file to read from */
  std::ifstream in_file;
  {
    std::lock_guard<std::mutex> lock(this->mut);
    /* try to open the file in binary read mode */
    in_file.open(filepath, std::ios::in | std::ios::binary);
    /* check if file could be opened */
    if (in_file.is_open()) {

        /* read the whole file to the vector */
        std::for_each(std::istreambuf_iterator<char>(in_file),
                      std::istreambuf_iterator<char>(),
                      [&file_data](const char c) {
                        file_data.push_back(c);
                      });

      /* make a DataPackage as response */
      response_package = std::make_shared<sockophil::DataPackage>(file_data, filename);
    } else {
      /* the requested file doesn't exist */
      response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::FILE_NOT_FOUND);
    }
    in_file.close();
  }
  /* send the response */
  this->send_package(accepted_socket, response_package);
}

bool LDAP_login(std::string username, std::string password){
  LDAP *ld, *ld2;           /* ldap resources */
  LDAPMessage *result, *e;  /* LPAD results */

  int rc = 0;            /* variables for bind and */

  char *attribs[3];        /* attribute array for search */
  attribs[0] = strdup("uid");        /* return uid and cn of entries */
  attribs[1] = strdup("cn");
  attribs[2] = NULL;        /* array must be NULL terminated */

  if ((ld = ldap_init(sockophil::HOST,sockophil::PORT)) == NULL){
    perror("LDAP init failed");
    return false;
  }

  std::cout << "Connected to LDAP server " <<  sockophil::HOST << "on Port " << sockophil::PORT << std::endl;

  /* first we bind anonymously */
  rc = ldap_simple_bind_s(ld, sockophil::BIND_USER, sockophil::BIND_PW);
  if(rc == LDAP_SUCCESS){
    std::cout << "Bind successful" << std::endl;
  }

  std::stringstream ss;
  ss << "(uid=" << username << "*)";

  rc = ldap_search_s(ld, sockophil::SEARCHBASE, sockophil::SCOPE, ss.str().c_str(), attribs, 0, &result );
  if(rc != LDAP_SUCCESS){
    std::cout << "LDAP search error: " << ldap_err2string(rc) << std::endl;
    return false;
  }
  int nrOfRecords = ldap_count_entries(ld, result);

  if (nrOfRecords > 0){
    struct termios term, term_orig;
    tcgetattr(STDERR_FILENO, &term);
    term_orig = term;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);


    tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);

    rc = 0;
    e = ldap_first_entry(ld, result);
    char * dn;
    if ((dn = ldap_get_dn( ld, e )) != NULL ) {
      std::cout << "dn: " << dn << std::endl;
      /* rebind */
      ld2 = ldap_init(sockophil::HOST, sockophil::PORT);
      rc = ldap_simple_bind_s(ld2, dn, password.c_str());
      std::cout << rc << std::endl;
      if (rc != 0) {
        std::cout << "Failed!" << std::endl;
      } else {
        std::cout << "Works!" << std::endl;
        ldap_unbind(ld2);
      }
      ldap_memfree(dn);
    }
  } else {
    std::cout << "User not found" << std::endl;
  }
  /* free memory used for result */
  ldap_msgfree(result);
  free(attribs[0]);
  free(attribs[1]);
  std::cout << "LDAP search done!" << std::endl;

  ldap_unbind(ld);
  return true;
}

}