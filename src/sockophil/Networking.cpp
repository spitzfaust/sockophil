//
// Created by tobias on 19/10/16.
//

#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cerrno>
#include <netinet/in.h>
#include <mutex>
#include "cereal/archives/json.hpp"
#include "sockophil/ErrnoExceptions.h"
#include "sockophil/Constants.h"
#include "sockophil/Helper.h"
#include "sockophil/Networking.h"

namespace sockophil {

Networking::~Networking() {}

/**
 * @brief Send a package over a given socket
 * @param socket_descriptor is the number of the socket
 * @param package is a pointer to a package
 */
void Networking::send_package(int socket_descriptor, const std::shared_ptr<Package> package) {
  /* serialising the package */
  std::stringstream ss;
  /* need a block for Cereal to work correctly */
  {
    std::lock_guard<std::mutex> lock(this->mut);
    cereal::PortableBinaryOutputArchive oarchive(ss);
    oarchive(package);
  }
  /* get a string from the serialised package */
  std::string data = ss.str();
  /* create the header from the size of the package and delimiters */
  auto data_size = data.size();
  this->send_header(socket_descriptor, data_size);

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
std::shared_ptr<Package> Networking::receive_package(int socket_descriptor) {
  /* stores the read data */
  std::vector<uint8_t> incoming;
  /* size of the received data */
  ssize_t size = 0;
  /* size of the package that should be received */
  long size_of_incoming = this->receive_header(socket_descriptor);
  long total_size = size_of_incoming;
  /* buffer for the blocks */
  uint8_t *buffer = nullptr;

  do {
    /* no header was received */
    if (size_of_incoming > 0 && size_of_incoming < BUF) {
      /* less than BUF size chars are left to read from the socket */
      buffer = new uint8_t[size_of_incoming];
      size = recv(socket_descriptor, buffer, size_of_incoming, 0);
    } else {
      /* read the full BUF size */
      buffer = new uint8_t[BUF];
      size = recv(socket_descriptor, buffer, BUF, 0);
    }
    /* something was received */
    if (size > 0) {
      /* receive data until size of incoming is 0 */
      for (unsigned long i = 0; (i < size) && (size_of_incoming > 0); ++i) {
        incoming.push_back(buffer[i]);
        --size_of_incoming;
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
          std::lock_guard<std::mutex> lock(this->mut);
          cereal::PortableBinaryInputArchive iarchive(data_stream);
          iarchive(pkg);
        }
        return pkg;
      }

    } else {
      /* something went wrong while reading from socket */
      throw SocketReceiveException(errno);
    }
  } while (true);
}

/**
 * @brief Send a header (long)
 * @param socket_descriptor is the number of the socket
 * @param header is the header that should be sent
 */
void Networking::send_header(const int &socket_descriptor, long header) const {
  header = htonl(header);
  send(socket_descriptor, &header, sizeof(header), 0);
}

/**
 * @brief Receive a header (long)
 * @param socket_descriptor is the number of the socket
 * @return the header
 * @throws SocketReceiveException if socket could not be read
 */
long Networking::receive_header(const int &socket_descriptor) const {
  long header = 0;
  if (recv(socket_descriptor, &header, sizeof(header), 0) > 0) {
    return ntohl(header);
  } else {
    /* something went wrong while reading from socket */
    throw SocketReceiveException(errno);
  }
}
/**
 * @brief Receive a file in chunks and store it
 * @param socket_descriptor is the number of the socket
 * @param output_file is the filestream to write to
 * @param call is a function that is called after a chunk is received (progress bar)
 */
void Networking::socket_store_file(const int &socket_descriptor, std::ofstream &output_file,
                                   const std::function<void(const long &,
                                                            const long &)> &call) const {
  /* size of the file that should be received */
  long filesize = receive_header(socket_descriptor);
  /* remaining bytes to read */
  long remaining = filesize;
  /* bytes that where already received */
  long total_received = 0;

  /* run until remaining is 0 */
  while (total_received != filesize) {
    /* number of bytes that was received */
    ssize_t size = 0;
    /* chunk (array) that will be read */
    uint8_t *chunk = nullptr;
    /* size of the chunk */
    long chunk_size = 0;
    if (remaining > 0 && remaining < BUF) {
      /* less than BUF size chars are left to read from the socket */
      chunk_size = remaining;
      chunk = new uint8_t[remaining];
    } else {
      /* read the full BUF size */
      chunk_size = BUF;
      chunk = new uint8_t[BUF];
    }
    size = recv(socket_descriptor, chunk, chunk_size, 0);
    if (size > 0) {
      total_received += chunk_size;
      call(total_received, filesize);
      output_file.write((char *) chunk, chunk_size);
      remaining -= chunk_size;
    } else {
      delete[] chunk;
      /* something went wrong while reading from socket */
      throw SocketReceiveException(errno);
    }
    delete[] chunk;
  }
}
/**
 * @brief Receive a file in chunks and store it.
 * @param socket_descriptor is the number of the socket
 * @param output_file is the filestream to write to
 */
void Networking::socket_store_file(const int &socket_descriptor, std::ofstream &output_file) const {
  /* just call the socket_store_file function with an empty closure */
  this->socket_store_file(socket_descriptor, output_file, [](const long &current, const long &total) {});
}

void Networking::socket_send_file(const int &socket_descriptor,
                                  std::ifstream &input_file) const {
  this->socket_send_file(socket_descriptor, input_file, [](const long &current, const long &total){});
}

/**
 * @brief Send a file in chunks over a socket.
 * @param socket_descriptor is the number of the socket
 * @param input_file is the filestream that is read from
 */
void Networking::socket_send_file(const int &socket_descriptor,
                                  std::ifstream &input_file,
                                  const std::function<void(const long &,
                                                           const long &)> &call) const {
  std::streampos filesize = 0;
  long allready_sent = 0;
  filesize = input_file.tellg();
  input_file.seekg(0, std::ios::end);
  filesize = input_file.tellg() - filesize;
  input_file.clear();
  input_file.seekg(0, std::ios::beg);
  if (filesize >= 0) {
    this->send_header(socket_descriptor, filesize);
  } else {
    /** @todo own exception */
    throw "send file error";
  }
  std::array<uint8_t, BUF> buffer;
  while (input_file.read((char *) buffer.data(), buffer.size())) {
    send(socket_descriptor, buffer.data(), buffer.size(), 0);
    allready_sent += buffer.size();
    call(allready_sent, (long) filesize);
  }
  std::streamsize size_read = input_file.gcount();
  std::vector<uint8_t> remaining(buffer.begin(), buffer.begin() + size_read);
  send(socket_descriptor, remaining.data(), remaining.size(), 0);
  allready_sent += remaining.size();
  if(size_read > 0) {
    call(allready_sent, (long) filesize);
  }
}
}
