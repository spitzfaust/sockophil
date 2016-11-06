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
#include <termios.h>
#include "sockserver/Server.h"
#include "sockophil/Helper.h"
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/ErrnoExceptions.h"
#include "sockophil/ListPackage.h"
#include "sockophil/Package.h"
#include "sockophil/FileInfoPackage.h"
#include "sockophil/ActionPackage.h"
#include "sockophil/ErrorPackage.h"
#include "sockophil/Constants.h"

#include "ldap.h"

#define LDAP_HOST "ldap.technikum-wien.at"
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define FILTER "(uid=if15b029*)"
#define BIND_USER NULL
#define BIND_PW NULL

namespace sockserver {

/**
 * @brief Constructor that stores member vars and sets up the socket
 * @param port is the port that should be listend on
 * @param target_directory is the directory to upload files
 */
Server::Server(unsigned short port, std::string target_directory) : port(port) {
  this->pool = std::make_unique<ThreadPool>();
  this->target_directory = sockophil::Helper::add_trailing_slash(target_directory);
  this->client_logins;
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
      /* thread begins */
      this->pool->schedule([this, accepted_socket, client_address](const std::atomic_bool &stop) {
        /* Bool for to check if client is logged in */
        bool authorized = false;
        /* Int that takes up number of logins */
        int login_tries = 0;
        /* Client IPAdress */
        std::string ip_Adress(inet_ntoa(client_address.sin_addr));
        std::string logindata;
        std::string username = "";
        std::string password = "";
        /* splitting the filename to username and password */
        bool splitter = false;

        while (true) {
          if (stop) {
            close(accepted_socket);
            return;
          }
          /* getting number of logins
           * or making a new entry */
          {
            std::lock_guard<std::mutex> lock(this->mut);
            if(this->client_logins.count(ip_Adress) > 0){
              login_tries = 4;
            }
          }
          /* package that was sent by the client to the server */
          auto received_pkg = this->receive_package(accepted_socket);
          /* a ActionPackage should be received */
          if (received_pkg->get_type() == sockophil::ACTION_PACKAGE && authorized) {
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
          }else if (login_tries <= 3){
            switch (std::static_pointer_cast<sockophil::ActionPackage>(received_pkg)->get_action()){
              case sockophil::LOGIN:
                logindata = std::static_pointer_cast<sockophil::ActionPackage>(received_pkg)->get_filename();
                username = "";
                password = "";
                /* splitting the filename to username and password */
                splitter = false;
                for (char p : logindata) {
                  if (p == '/') {
                    splitter = true;
                  }else {
                    if (splitter) {
                      password += p;
                    } else {
                      username += p;
                    }
                  }
                }
                if (LDAP_login(username, password)) {
                  authorized = true;
                  std::cout << "fuck yeah" << std::endl;
                  /* TODO send back correct input input */
                } else {
                  ++login_tries;
                  /* TODO send back incorrect input
                   */
                }
                break;
              case sockophil::LIST:
                /* not logged in package */
                break;
              case sockophil::PUT:
                /* not logged in package */
                break;
              case sockophil::GET:
                /* not logged in package */
                break;
              case sockophil::QUIT:
                close(accepted_socket);
                return;
            }
          }else{
            {
              std::lock_guard<std::mutex> lock(this->mut);
              if(this->client_logins.count(ip_Adress) > 0){
                /* TODO tell client he cant login till time + 30 min */
              }else {
                time_t now;
                time(&now);
                this->client_logins[ip_Adress] = now;
                /* TODO tell client he cant login till time + 30 min */
              }
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
  bool file_opened = false;
  /* Package for the response */
  std::shared_ptr<sockophil::Package> response_package = nullptr;
  std::shared_ptr<sockophil::FileInfoPackage> data_package = nullptr;
  /* try to receive a FileInfoPackage */
  auto received_pkg = this->receive_package(accepted_socket);
  /* should be a FileInfoPackage */
  if (received_pkg->get_type() == sockophil::FILE_INFO_PACKAGE) {
    /* cast to a FileInfoPackage */
    data_package = std::static_pointer_cast<sockophil::FileInfoPackage>(received_pkg);
    /* file to write to */
    std::ofstream output_file;
    this->add_file_mutex(data_package->get_filename());
    {
      std::lock_guard<std::mutex> lock(*this->file_muts[data_package->get_filename()]);
      /* try to open t<< "-"he file */
      output_file.open(this->target_directory + data_package->get_filename(), std::ios::out | std::ios::binary);
      /* check if file could be opened */
      file_opened = output_file.is_open();
      if (file_opened) {
        /* write to the file and create SuccessPackage */
        this->socket_store_file(accepted_socket, output_file);
        response_package = std::make_shared<sockophil::SuccessPackage>();
      } else {
        /* file could not be stored */
        response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::FILE_STORAGE);
      }
      output_file.close();
    }
  } else {
    /* was not the expected FileInfoPackage */
    response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::WRONG_PACKAGE);
  }
  /* send the response to the client */
  this->send_package(accepted_socket, response_package);
  if(!file_opened) {
    this->remove_file_mutex(data_package->get_filename());
  }
}

/**
 * @brief Send a requested file to the client
 * @param accepted_socket is the socket that is used to communicate over
 * @param filename is the requested file
 */
void Server::return_file(int accepted_socket, std::string filename) {
  bool file_opened;
  std::shared_ptr<sockophil::Package> response_package = nullptr;
  std::string filepath = this->target_directory + filename;
  /* file to read from */
  std::ifstream in_file;
  this->add_file_mutex(filename);
  {
    std::lock_guard<std::mutex> lock(*this->file_muts[filename]);
    /* try to open the file in binary read mode */
    in_file.open(filepath, std::ios::in | std::ios::binary);
    /* check if file could be opened */
    file_opened = in_file.is_open();
    if (file_opened) {
      /* make a FileInfoPackage as response */
      response_package = std::make_shared<sockophil::FileInfoPackage>(filename);
      this->send_package(accepted_socket, response_package);
      this->socket_send_file(accepted_socket, in_file);
    } else {
      /* the requested file doesn't exist */
      response_package = std::make_shared<sockophil::ErrorPackage>(sockophil::FILE_NOT_FOUND);
      this->send_package(accepted_socket, response_package);
    }
    in_file.close();
  }
  if(!file_opened) {
    this->remove_file_mutex(filename);
  }
  /* send the response */
}

void Server::add_file_mutex(std::string filename) {
  {
    std::lock_guard<std::mutex> lock(this->mut);
    if (this->file_muts.find(filename) == this->file_muts.end()) {
      file_muts.emplace(std::make_pair(filename, std::make_unique<std::mutex>()));
    }
  }
}

void Server::remove_file_mutex(std::string filename) {
  {
    std::lock_guard<std::mutex> lock(this->mut);
    file_muts.erase(filename);
  }
}

bool Server::LDAP_login(std::string username, std::string password){
  LDAP *ld, *ld2;           /* ldap resources */
  LDAPMessage *result, *e;  /* LPAD results */

  int rc = 0;            /* variables for bind and */

  char *attributes[3];        /* attribute array for search */
  attributes[0] = strdup("uid");        /* return uid and cn of entries */
  attributes[1] = strdup("cn");
  attributes[2] = NULL;        /* array must be NULL terminated */

  if ((ld = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL){
    perror("LDAP init failed");
    return false;
  }
  /* first we bind anonymously */
  rc = ldap_simple_bind_s(ld, BIND_USER, BIND_PW);

  std::stringstream ss;
  ss << "(uid=" << username << "*)";
  rc = ldap_search_s(ld, SEARCHBASE, SCOPE, ss.str().c_str(), attributes, 0, &result );
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
      /* rebind */
      ld2 = ldap_init(LDAP_HOST, LDAP_PORT);
      rc = ldap_simple_bind_s(ld2, dn, password.c_str());
      if (rc != 0) {
        std::cout << "Username or Password incorrect!" << std::endl;
        return false;
      }
      ldap_unbind(ld2);
      ldap_memfree(dn);
    }
  } else {
    std::cout << "Username or Password incorrect!" << std::endl;
    return false;
  }

  ldap_unbind(ld);
  return true;
}

}