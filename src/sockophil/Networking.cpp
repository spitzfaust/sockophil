//
// Created by tobias on 19/10/16.
//

#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <cerrno>
#include "cereal/archives/portable_binary.hpp"
#include "sockophil/SocketReceiveException.h"
#include "sockophil/constants.h"
#include "sockophil/Helper.h"
#include "sockophil/Networking.h"

namespace sockophil {
    void Networking::send_package(int socket_descriptor, const std::shared_ptr <Package> package) const {
        std::stringstream ss; // any stream can be used

        {
            cereal::PortableBinaryOutputArchive oarchive(ss);
            oarchive(package);
        }
        std::string data = ss.str();
        std::string data_size = SIZE_DELIM + std::to_string(data.size()) + SIZE_DELIM;
        data_size = data_size.insert(1, HEADER_SIZE - data_size.size(), '0');
        //std::cout << "data_size: " << data_size << std::endl;
        send(socket_descriptor, data_size.c_str(), data_size.size(), 0);

        for (unsigned i = 0; i < data.length(); i += BUF) {
            //std::cout << i << std::endl;
            auto data_part = data.substr(i, BUF);
            std::vector<char> buffer(data_part.begin(), data_part.end());
            //std::cout << "buffer size: " << buffer.size() << std::endl;
            send(socket_descriptor, buffer.data(), buffer.size(), 0);
        }
    }

    std::shared_ptr<Package> Networking::receive_package(int socket_descriptor) const {
        std::vector<char> incoming;
        ssize_t size = 0;

        long size_of_incoming = 0;
        char * buffer = nullptr;
        do {
            if(size_of_incoming == 0) {
                buffer = new char[HEADER_SIZE];
                size = recv(socket_descriptor, buffer, HEADER_SIZE, 0);
            } else if(size_of_incoming > 0 && size_of_incoming < BUF) {
                //std::cout << "-- 1 FIRST IF " << std::endl;
                buffer = new char[size_of_incoming];
                size = recv(socket_descriptor, buffer, size_of_incoming, 0);
            } else {
                //std::cout << "-- 1 SECOND IF " << std::endl;
                buffer = new char[BUF];
                size = recv(socket_descriptor, buffer, BUF, 0);
            }
            if (size > 0) {
                if(size_of_incoming == 0){
                    std::stringstream ss;
                    char first_delim = '\0';
                    unsigned int number_of_digits = 0;
                    ss << buffer;
                    ss.get(first_delim);
                    if(first_delim == SIZE_DELIM[0]) {
                        ss >> size_of_incoming;
                        //std::cout << "-- 1 size_of_incoming " << size_of_incoming << std::endl;
                        //std::cout << "-- 1 size " << size << std::endl;
                    }
                } else {
                    //std::cout << "-- 2 SIZE " << size << std::endl;
                    for (unsigned long i = 0; (i < size) && (size_of_incoming > 0); ++i) {
                       // std::cout << "-- 2 for soi: " << size_of_incoming << std::endl;
                       // std::cout << "-- 2 for i:   " << i << std::endl;
                        incoming.push_back(buffer[i]);
                       // std::cout << "-- 2 buf:     " << buffer[i] << std::endl;
                        --size_of_incoming;
                    }
                }
                delete[] buffer;

                if(size_of_incoming == 0) {
                    std::shared_ptr<sockophil::Package> pkg;
                    //std::cout << "Size of incoming: " << incoming.size() << std::endl;
                    std::stringstream data_stream(std::string(incoming.begin(), incoming.end()));
                    {
                        cereal::PortableBinaryInputArchive iarchive(data_stream);
                        iarchive(pkg);
                    }
                    incoming.clear();
                    return pkg;
                }

            }  else {
                throw SocketReceiveException(errno);
            }

        } while (true);
    }
}