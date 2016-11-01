//
// Created by tobias on 19/10/16.
//

#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <cerrno>
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/ErrnoExceptions.h"
#include "sockophil/Constants.h"
#include "sockophil/Helper.h"
#include "sockophil/Networking.h"

namespace sockophil {
/**
 * @brief Send a package over a given socket
 * @param socket_descriptor is the number of the socket
 * @param package is a pointer to a package
 */
void Networking::send_package(int socket_descriptor, const std::shared_ptr<Package> package) const {
  /* serialising the package */
  std::stringstream ss;
  /* need a block for Cereal to work correctly */
  {
    cereal::PortableBinaryOutputArchive oarchive(ss);
    oarchive(package);
  }
  /* get a string from the serialised package */
  std::string data = ss.str();
  /* create the header from the size of the package and delimiters */
  std::string data_size = SIZE_DELIM + std::to_string(data.size()) + SIZE_DELIM;
  /* fill the header up with 0's to get to the fixed header size */
  data_size = data_size.insert(1, HEADER_SIZE - data_size.size(), '0');

  /* send the header to the server */
  send(socket_descriptor, data_size.c_str(), data_size.size(), 0);

  /* send the package in BUF sized blocks */
  for (unsigned i = 0; i < data.length(); i += BUF) {
    auto data_part = data.substr(i, BUF);
    std::vector<char> buffer(data_part.begin(), data_part.end());
    send(socket_descriptor, buffer.data(), buffer.size(), 0);
  }
}

/**
 * @brief Receive a package on the given socket
 * @param socket_descriptor is the number of the socket
 * @return a received package
 */
std::shared_ptr<Package> Networking::receive_package(int socket_descriptor) const {
  /* stores the read data */
  std::vector<char> incoming;
  /* size of the received data */
  ssize_t size = 0;
  /* size of the package that should be received */
  long size_of_incoming = 0;
  /* buffer for the blocks */
  char *buffer = nullptr;
  do {
    /* no header was received */
    if (size_of_incoming == 0) {
      /* only get HEADER_SIZE chars from the socket */
      buffer = new char[HEADER_SIZE];
      size = recv(socket_descriptor, buffer, HEADER_SIZE, 0);
    } else if (size_of_incoming > 0 && size_of_incoming < BUF) {
      /* less than BUF size chars are left to read from the socket */
      buffer = new char[size_of_incoming];
      size = recv(socket_descriptor, buffer, size_of_incoming, 0);
    } else {
      /* read the full BUF size */
      buffer = new char[BUF];
      size = recv(socket_descriptor, buffer, BUF, 0);
    }
    /* something was received */
    if (size > 0) {
      /* receive header first */
      if (size_of_incoming == 0) {
        /* storing the size of the incoming data */
        std::stringstream ss;
        char first_delim = '\0';
        ss << buffer;
        ss.get(first_delim);
        if (first_delim == SIZE_DELIM[0]) {
          ss >> size_of_incoming;
        }
        // @todo error handling
      } else {
        /* receive data until size of incoming is 0 */
        for (unsigned long i = 0; (i < size) && (size_of_incoming > 0); ++i) {
          incoming.push_back(buffer[i]);
          --size_of_incoming;
        }
      }
      /* don't forget to free memory */
      delete[] buffer;

      /* if everything is received */
      if (size_of_incoming == 0) {
        /* pointer to store the deserialised Package */
        std::shared_ptr<sockophil::Package> pkg;
        /* create a stringstream from the received data */
        std::stringstream data_stream(std::string(incoming.begin(), incoming.end()));
        /* need a block for Cereal to work correctly */
        {
          cereal::PortableBinaryInputArchive iarchive(data_stream);
          iarchive(pkg);
        }
        // @todo check if clear is necasary
        incoming.clear();
        return pkg;
      }

    } else {
      /* something went wrong while reading from socket */
      throw SocketReceiveException(errno);
    }
  } while (true);
}
}